// cbj_vessel_v1_1_0.scad
// Parametric 28.62 cm ID Chiral Bell Jar Cavity (21-Port) & Helmholtz Mounts
// Author: Justin D. Grimm (ORCID: 0009-0000-5299-0243) | Version: v1.1.0
// Units: millimeters (mm) | Handedness: Right-handed Z-up, CCW azimuth (+phi)

$fn = 72;

// --- 1. Core Metrological Invariants ---
R_IN          = 143.10;  // TM101 915.0 MHz inner radius (25 um OFHC Cu inner skin)
WALL_T        = 3.00;    // Annealed non-magnetic 304L stainless wall
R_OUT         = R_IN + WALL_T;
R_BOSS        = 178.00;  // Concentric metrology sphere for all flange faces
SPLIT_DEG     = 72.00;   // Non-equatorial dome split (clears 90 deg probe ring)
Z_SPLIT       = R_IN * cos(SPLIT_DEG); // +44.22 mm parting plane

// Flange Standards: [bore_dia, tube_od, flange_od, flange_thick]
FLG_DN40CF    = [38.10, 41.30, 69.85, 12.70]; // 2.75 in ConFlat
FLG_DN16CF    = [16.00, 19.05, 33.78,  7.62]; // 1.33 in Mini-ConFlat
FLG_KF25      = [22.00, 25.40, 40.00,  5.50]; // Screened Vacuum KF-25

// Polar Angles (deg)
THETA_MAGIC   = 54.7356;  // arccos(1/sqrt(3)) -> P2(cos theta) = 0 (Am-241)
THETA_EQ      = 90.0000;  // 13-node homodyne pickup ring
THETA_TET     = 109.4712; // arccos(-1/3) -> lower tetrahedral drives
THETA_VAC     = 135.0000; // Screened scroll-pump & Argon ports

// Helmholtz Coil Pair (28.0 mT at 10.0 A, 685 turns)
R_HELM        = 220.00;
Z_HELM        = R_HELM / 2.0; // +/- 110.00 mm (polar ray 63.43 deg)
COIL_W        = 28.00;
COIL_H        = 28.00;

SECTION_CUT   = true;     // Set false for watertight 3D print / STEP export
EXPLODE_CAP   = 0.0;      // Set > 0 (e.g. 35.0) to lift upper dome cap

// --- 2. Orientation & Nozzle Primitives ---
module orient_spherical(theta_deg, phi_deg) {
    rotate([0, 0, phi_deg])
        rotate([0, theta_deg, 0])
            children();
}

module port_nozzle_solid(spec, r_face) {
    tube_od = spec[1];
    flg_od  = spec[2];
    flg_t   = spec[3];
    orient_spherical(0, 0) {
        cylinder(h = r_face - flg_t, d = tube_od);
        translate([0, 0, r_face - flg_t])
            cylinder(h = flg_t, d = flg_od);
    }
}

module port_nozzle_bore(spec, r_face) {
    bore_d = spec[0];
    cylinder(h = r_face + 2.0, d = bore_d);
}

// Unified 21-Port Placement Schedule (mode = "solid" or "bore")
module all_21_ports(mode = "solid", upper_only = false, lower_only = false) {
    module place_port(theta, phi, spec, label, trim_mm = 0.0) {
        is_upper = (theta < SPLIT_DEG);
        if ((!upper_only && !lower_only) ||
            (upper_only && is_upper) ||
            (lower_only && !is_upper)) {
            orient_spherical(theta, phi) {
                if (mode == "solid")
                    port_nozzle_solid(spec, R_BOSS + trim_mm);
                else
                    port_nozzle_bore(spec, R_BOSS + trim_mm);
            }
        }
    }

    // 1. North Tetrahedral Drive + SHIL + HV Anode (Port T0)
    place_port(0.0, 0.0, FLG_DN40CF, "T0_NORTH");

    // 2. Magic-Angle Americium-241 Degrader & Guard PIN (Port ISO-Am)
    place_port(THETA_MAGIC, 0.0, FLG_DN40CF, "ISO_AM241");

    // 3. 13x Equatorial Homodyne I/Q Pickups (CCW P00..P12) + Firmware LUT Emit
    for (k = [0 : 12]) {
        az_deg  = k * (360.0 / 13.0);
        trim_mm = 0.00; // Nominal radial standoff delta for C99 Q2.14 LUT
        if (mode == "solid" && !upper_only)
            echo("CBJ_CAD_LUT", k, az_deg, R_BOSS + trim_mm);
        place_port(THETA_EQ, az_deg, FLG_DN16CF, str("P", k), trim_mm);
    }

    // 4. 3x Lower Tetrahedral RF Drives (Ports T1..T3)
    for (m = [0 : 2]) {
        place_port(THETA_TET, 13.8462 + m * 120.0, FLG_DN16CF, str("T", m + 1));
    }

    // 5. 2x Honeycomb-Screened KF-25 Vacuum & Argon Ports (Ports V1..V2)
    place_port(THETA_VAC,  73.8462, FLG_KF25, "V1_SCROLL_PUMP");
    place_port(THETA_VAC, 193.8462, FLG_KF25, "V2_ARGON_INLET");

    // 6. South Axial Cobalt-60 Chiral Beta Collimator (Port ISO-Co)
    place_port(180.0, 0.0, FLG_DN40CF, "ISO_CO60");
}

// --- 3. Monolithic Lower Bowl & 72-Degree Upper Dome Cap ---
module latitude_parting_collar(z_pos) {
    r_ring_in  = sqrt(R_IN * R_IN - z_pos * z_pos);
    r_ring_out = r_ring_in + 22.0;
    translate([0, 0, z_pos - 10.0])
        difference() {
            cylinder(h = 20.0, r = r_ring_out);
            translate([0, 0, -1.0])
                cylinder(h = 22.0, r = r_ring_in);
            translate([0, 0, 10.0])
                rotate_extrude()
                    translate([r_ring_in + 8.5, 0, 0])
                        circle(d = 3.5);
        }
}

module bell_jar_vessel() {
    color([0.76, 0.78, 0.82])
    difference() {
        union() {
            intersection() {
                sphere(r = R_OUT);
                translate([0, 0, -250 + Z_SPLIT]) cube([500, 500, 500], center = true);
            }
            intersection() {
                latitude_parting_collar(Z_SPLIT);
                translate([0, 0, -250 + Z_SPLIT]) cube([500, 500, 500], center = true);
            }
            all_21_ports(mode = "solid", lower_only = true);
        }
        sphere(r = R_IN);
        all_21_ports(mode = "bore", lower_only = true);
    }

    color([0.82, 0.84, 0.88])
    translate([0, 0, EXPLODE_CAP])
    difference() {
        union() {
            intersection() {
                sphere(r = R_OUT);
                translate([0, 0, 250 + Z_SPLIT]) cube([500, 500, 500], center = true);
            }
            intersection() {
                latitude_parting_collar(Z_SPLIT);
                translate([0, 0, 250 + Z_SPLIT]) cube([500, 500, 500], center = true);
            }
            all_21_ports(mode = "solid", upper_only = true);
        }
        sphere(r = R_IN);
        all_21_ports(mode = "bore", upper_only = true);
    }

    // Central Sub-Breakdown Townsend Guard Anode Ring (OFHC Copper, r = 14 mm)
    color([0.88, 0.52, 0.22])
    rotate_extrude()
        translate([14.0, 0, 0])
            circle(d = 2.0);
}

// --- 4. Non-Magnetic Helmholtz Coil Pair & G10 Standoff Rig ---
module helmholtz_assembly() {
    color([0.78, 0.44, 0.16])
    for (sgn = [-1, 1]) {
        translate([0, 0, sgn * Z_HELM - COIL_H / 2.0])
            difference() {
                cylinder(h = COIL_H, r = R_HELM + COIL_W / 2.0);
                translate([0, 0, -1.0])
                    cylinder(h = COIL_H + 2.0, r = R_HELM - COIL_W / 2.0);
            }
    }
    color([0.35, 0.58, 0.42])
    for (n = [0 : 3]) {
        rotate([0, 0, 45.0 + n * 90.0])
            translate([R_HELM, 0, -Z_HELM - COIL_H / 2.0])
                cylinder(h = 2.0 * Z_HELM + COIL_H, d = 14.0);
    }
}

difference() {
    union() {
        bell_jar_vessel();
        helmholtz_assembly();
    }
    if (SECTION_CUT) {
        translate([0, -300, 0])
            cube([600, 600, 600], center = true);
    }
}
