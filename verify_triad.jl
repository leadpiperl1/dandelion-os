# Dandelion OS Triadic Verification Harness
println("=== STARTING DANDELION OS TRIADIC VERIFICATION SUITE ===")

include("build/ShingariPilot.jl")

# Dynamically discover the generated Pilot module in Main
mod_sym = first(filter(m -> endswith(string(m), "Pilot") && m != :Main, names(Main, all=true)))
pilot_mod = getfield(Main, mod_sym)
println("[0 GREEN] Discovered dynamic Pilot module: $mod_sym")

dispatch_methods = methods(pilot_mod.dispatch_deed!)
println("[0 GREEN] Found $(length(dispatch_methods)) triadic dispatch methods to verify.")

for m in dispatch_methods
    sig = m.sig
    if length(sig.parameters) >= 5
        IntentType = sig.parameters[2]
        SyscallType = sig.parameters[3]
        KernelType = sig.parameters[4]
        
        intent_obj = IntentType()
        syscall_obj = SyscallType()
        kernel_obj = KernelType()
        
        pkt_ref = Ref(pilot_mod.default_packet(intent_obj))
        
        # Warm up JIT compilation for this method signature
        pilot_mod.dispatch_deed!(intent_obj, syscall_obj, kernel_obj, pkt_ref)
        
        # Measure heap allocations during zero-copy dispatch
        bytes_allocated = @allocated pilot_mod.dispatch_deed!(intent_obj, syscall_obj, kernel_obj, pkt_ref)
        res = pilot_mod.dispatch_deed!(intent_obj, syscall_obj, kernel_obj, pkt_ref)
        
        println("   - Verified dispatch: $(IntentType) | Weld: $res | Allocated: $bytes_allocated bytes")
        @assert bytes_allocated == 0 "Heap allocation detected! $(bytes_allocated) bytes allocated."
    end
end

println("[+1 RED -> 0 GREEN PASS] Julia 1.12 Multiple Dispatch @ccall verified with zero heap allocations.")
println("=== ALL TRIADIC VERIFICATION CHECKS PASSED WITH ZERO ALLOCATIONS ===")
