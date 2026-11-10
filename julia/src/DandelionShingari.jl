module DandelionShingari

export Trit, FowlerSeed40, vbst_dot40, verify_keyhole_variance

const LIB_DANDELION = joinpath(@__DIR__, "..", "..", "build", "libdandelion_substrate.so")

@enum Trit::Int8 begin
    BLUE_SUBJ = -1
    GREEN_OBJ = 0
    RED_ACTION = 1
end

struct FowlerSeed40
    pos_mask::UInt64
    neg_mask::UInt64
end

@inline function vbst_dot40(a::FowlerSeed40, b::FowlerSeed40)::Int32
    return ccall(
        (:vbst_dot40_popcnt, LIB_DANDELION),
        Int32,
        (UInt64, UInt64, UInt64, UInt64),
        a.pos_mask, a.neg_mask, b.pos_mask, b.neg_mask
    )
end

@inline function verify_keyhole_variance(shear::Float64)::Bool
    return abs(shear - (2.0 / 3.0)) <= 0.05
end

end # module
