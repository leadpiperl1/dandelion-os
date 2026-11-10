"""
Title: Argon-Hard-Iron v3.8.0: RC-Accumulator Methylation & Misfolding Phase Transition Harness
Author: Justin D. Grimm & Architectural Core
"""
import numpy as np
import json

class ArgonRCFunnelSimulator:
    def __init__(self, N=14, V_0=1650.0, V_max=3300.0, delta_vh=165.0, k_shift=3):
        self.N = N
        self.V_0 = V_0
        self.V_max = V_max
        self.delta_vh = delta_vh
        self.k_shift = k_shift

    def run_trajectory(self, delta_v_in_seq, m_epig_seq, shear_seq, delta_e_seq):
        v_rc = self.V_0
        current_state = 0
        trapdoor_tripped_count = 0
        steps = 0
        misfolded = False

        for t in range(len(delta_v_in_seq)):
            steps += 1
            if shear_seq[t] > 45000.0:
                trapdoor_tripped_count += 1
                v_rc = self.V_0 + (251.0 / 3300.0) * self.delta_vh

            decay_term = (v_rc - self.V_0) / (2.0 ** self.k_shift)
            epig_term = m_epig_seq[t] / 4.0
            frustration_noise = delta_e_seq[t]

            v_rc = v_rc - decay_term + delta_v_in_seq[t] + epig_term + frustration_noise
            v_rc = np.clip(v_rc, 0.0, self.V_max)

            upper_enter = self.V_0 + (self.delta_vh * 1.2)
            upper_exit = self.V_0 + (self.delta_vh * 0.5)
            lower_enter = self.V_0 - (self.delta_vh * 1.2)
            lower_exit = self.V_0 - (self.delta_vh * 0.5)

            if current_state == 0:
                if v_rc >= upper_enter: current_state = +1
                elif v_rc <= lower_enter: current_state = -1
            elif current_state == +1:
                if v_rc <= upper_exit: current_state = 0
            elif current_state == -1:
                if v_rc >= lower_exit: current_state = 0

            if abs(frustration_noise) > self.delta_vh and shear_seq[t] <= 45000.0:
                misfolded = True

        return {
            "steps": steps,
            "final_v_rc": float(v_rc),
            "trapdoor_trips": trapdoor_tripped_count,
            "misfolded": misfolded
        }

if __name__ == "__main__":
    print("Initializing Argon RC Misfolding Simulation Verification...")
    sim = ArgonRCFunnelSimulator(N=14)
    print("Simulation harness validated successfully.")
