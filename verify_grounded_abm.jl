#!/usr/bin/env julia
module GroundedABMVerification
using Printf
const Q16 = Int32
const Q16_ONE = Int32(1 << 16)
const LIB_PATH = joinpath(@__DIR__, "dandelion_grounded_abm", "target", "release", "libdandelion_grounded_abm.so")

struct GroundedAgent64B
    x_trit::NTuple{8, Int8}
    neighbors::NTuple{4, UInt16}
    mz_ring::NTuple{8, UInt8}
    ring_head::UInt8
    status_byte::UInt8
    _pad1::NTuple{2, UInt8}      # 2 bytes alignment padding
    latent_q16::NTuple{8, Int32}
    _pad2::NTuple{4, UInt8}      # Trailing padding to match 64-byte align(64) size
end

@assert sizeof(GroundedAgent64B) == 64 "ABI Error: GroundedAgent64B must be exactly 64 bytes"

function verify_simulation()
    if !isfile(LIB_PATH)
        error("Library missing at $LIB_PATH")
    end
    println("=== Dandelion Grounded ABM Verification Engine ===")
    N = 1024
    T = 200
    agents = Vector{GroundedAgent64B}(undef, N)
    for i in 1:N
        n1 = UInt16(mod(i - 2, N))
        n2 = UInt16(mod(i, N))
        n3 = UInt16(mod(i - 5, N))
        n4 = UInt16(mod(i + 3, N))
        init_val = Int8(mod(i, 3) - 1)
        trits = ntuple(_ -> init_val, 8)
        latents = ntuple(_ -> Int32(init_val) * Int32(20000), 8)
        agents[i] = GroundedAgent64B(trits, (n1, n2, n3, n4), ntuple(_ -> UInt8(121), 8), UInt8(0), UInt8(121), (0x00, 0x00), latents, (0x00, 0x00, 0x00, 0x00))
    end
    rng_ref = Ref{UInt32}(0x00000a42)
    omega_q16 = round(Q16, 0.15 * Q16_ONE)
    coupling_q16 = round(Q16, 0.25 * Q16_ONE)
    dt_q16 = round(Q16, 0.05 * Q16_ONE)
    saturation_counts = Vector{UInt32}(undef, T)
    
    t_start = time_ns()
    for t in 1:T
        saturation_counts[t] = GC.@preserve agents rng_ref begin
            ccall(
                (:dandelion_grounded_step, LIB_PATH),
                UInt32,
                (Ptr{GroundedAgent64B}, Csize_t, Q16, Q16, Q16, Ptr{UInt32}),
                pointer(agents), length(agents), omega_q16, coupling_q16, dt_q16, rng_ref
            )
        end
    end
    t_elapsed = (time_ns() - t_start) / 1e6
    @printf("[ OK ] Simulation complete in %.2f ms (%.2f us/step)\n", t_elapsed, t_elapsed * 1000 / T)
    @printf("Peak Keyhole Clamps (Byte 243): %d agents\n", maximum(saturation_counts))
    @printf("Final Agent[1] Trit State:     %s\n", string(agents[1].x_trit))
    @printf("Final Agent[1] Status Byte:    %d\n", agents[1].status_byte)
end
end
using .GroundedABMVerification
GroundedABMVerification.verify_simulation()
