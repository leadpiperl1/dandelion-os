`default_nettype none
/*
 * cbj_ice40_top_v1_1_0.v
 * Top-Level Lattice iCE40HX8K-CT256 Arbiter for the Chiral Bell Jar Processor
 * Author: Justin D. Grimm (ORCID: 0009-0000-5299-0243) | Version: v1.1.0
 */

module cbj_cordic_atan2_q13 (
    input  wire               clk,
    input  wire               rst_n,
    input  wire               start,
    input  wire signed [15:0] i_in,
    input  wire signed [15:0] q_in,
    output reg                done,
    output reg  signed [15:0] phi_q13
);
    reg        running;
    reg [3:0]  step;
    reg signed [19:0] x, y;
    reg signed [17:0] z;

    function signed [17:0] atan_q13_lut(input [3:0] k);
        case (k)
            4'd0:  atan_q13_lut = 18'sd6434;
            4'd1:  atan_q13_lut = 18'sd3798;
            4'd2:  atan_q13_lut = 18'sd2007;
            4'd3:  atan_q13_lut = 18'sd1019;
            4'd4:  atan_q13_lut = 18'sd511;
            4'd5:  atan_q13_lut = 18'sd256;
            4'd6:  atan_q13_lut = 18'sd128;
            4'd7:  atan_q13_lut = 18'sd64;
            4'd8:  atan_q13_lut = 18'sd32;
            4'd9:  atan_q13_lut = 18'sd16;
            4'd10: atan_q13_lut = 18'sd8;
            4'd11: atan_q13_lut = 18'sd4;
            4'd12: atan_q13_lut = 18'sd2;
            default: atan_q13_lut = 18'sd1;
        endcase
    endfunction

    wire signed [19:0] i_ext = {{4{i_in[15]}}, i_in};
    wire signed [19:0] q_ext = {{4{q_in[15]}}, q_in};

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            running <= 1'b0;
            step    <= 4'd0;
            x       <= 20'sd0;
            y       <= 20'sd0;
            z       <= 18'sd0;
            done    <= 1'b0;
            phi_q13 <= 16'sd0;
        end else begin
            done <= 1'b0;
            if (start) begin
                running <= 1'b1;
                step    <= 4'd0;
                if (i_ext == 20'sd0 && q_ext == 20'sd0) begin
                    running <= 1'b0;
                    done    <= 1'b1;
                    phi_q13 <= 16'sd0;
                end else if (i_ext >= 20'sd0) begin
                    x <= i_ext;
                    y <= q_ext;
                    z <= 18'sd0;
                end else if (q_ext >= 20'sd0) begin
                    x <=  q_ext;
                    y <= -i_ext;
                    z <=  18'sd12868;
                end else begin
                    x <= -q_ext;
                    y <=  i_ext;
                    z <= -18'sd12868;
                end
            end else if (running) begin
                if (y >= 20'sd0) begin
                    x <= x + (y >>> step);
                    y <= y - (x >>> step);
                    z <= z + atan_q13_lut(step);
                end else begin
                    x <= x - (y >>> step);
                    y <= y + (x >>> step);
                    z <= z - atan_q13_lut(step);
                end

                if (step == 4'd13) begin
                    running <= 1'b0;
                    done    <= 1'b1;
                    if (y >= 20'sd0)
                        phi_q13 <= z[15:0] + atan_q13_lut(4'd13);
                    else
                        phi_q13 <= z[15:0] - atan_q13_lut(4'd13);
                end else begin
                    step <= step + 4'd1;
                end
            end
        end
    end
endmodule

module cbj_spi_slave_64b (
    input  wire         clk,
    input  wire         rst_n,
    input  wire         spi_sck,
    input  wire         spi_cs_n,
    input  wire         spi_mosi,
    output reg          spi_miso,
    output wire         cs_active,
    output reg  [5:0]   rd_addr,
    input  wire [7:0]   rd_data
);
    reg [2:0] sck_sync;
    reg [2:0] csn_sync;
    reg [2:0] mosi_sync;
    reg [2:0] bit_idx;

    wire csn_fall = (csn_sync[2:1] == 2'b10);
    wire sck_fall = (sck_sync[2:1] == 2'b10);
    assign cs_active = ~csn_sync[1];

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            sck_sync  <= 3'b000;
            csn_sync  <= 3'b111;
            mosi_sync <= 3'b000;
            rd_addr   <= 6'd0;
            bit_idx   <= 3'd7;
            spi_miso  <= 1'b0;
        end else begin
            sck_sync  <= {sck_sync[1:0],  spi_sck};
            csn_sync  <= {csn_sync[1:0],  spi_cs_n};
            mosi_sync <= {mosi_sync[1:0], spi_mosi};

            if (csn_fall) begin
                rd_addr  <= 6'd0;
                bit_idx  <= 3'd7;
                spi_miso <= rd_data[7];
            end else if (!csn_sync[1]) begin
                if (sck_sync == 3'b000 && rd_addr == 6'd0 && bit_idx == 3'd7) begin
                    spi_miso <= rd_data[7];
                end
                if (sck_fall) begin
                    if (bit_idx == 3'd0) begin
                        bit_idx <= 3'd7;
                        rd_addr <= rd_addr + 6'd1;
                    end else begin
                        bit_idx  <= bit_idx - 3'd1;
                        spi_miso <= rd_data[bit_idx - 3'd1];
                    end
                end else if (bit_idx == 3'd7) begin
                    spi_miso <= rd_data[7];
                end
            end else begin
                rd_addr  <= 6'd0;
                bit_idx  <= 3'd7;
                spi_miso <= 1'b0;
            end
        end
    end
endmodule

module cbj_ice40_top #(
    parameter integer       CLK_HZ_DIV_1US = 40,
    parameter integer       DARK_MUTE_CYC  = 2000,
    parameter signed [31:0] R_NULL_SQ      = 32'sd65536
)(
    input  wire        clk_sys,
    input  wire        rst_n,
    input  wire        alpha_schmitt,
    output reg         adc_convst,
    output reg         adc_sclk,
    output reg         adc_reset,
    output reg  [3:0]  adc_cs_n,
    input  wire [3:0]  adc_busy,
    input  wire [3:0]  adc_miso,
    output reg         shil_env_gate,
    output reg         dark_cal_sw_ctl,
    input  wire        esp_fail_closed_n,
    output wire        hw_rf_hv_en,
    input  wire        spi1_sck,
    input  wire        spi1_cs_n,
    input  wire        spi1_mosi,
    output wire        spi1_miso,
    output reg         erlang_strobe_a,
    input  wire        leg1_start,
    output wire        leg1_busy,
    input  wire        leg1_done,
    input  wire        spi2_sck,
    input  wire        spi2_cs_n,
    input  wire        spi2_mosi,
    output wire        spi2_miso,
    output reg         erlang_strobe_c,
    input  wire        leg2_start,
    output wire        leg2_busy,
    input  wire        leg2_done,
    input  wire        exciton_fpga_in,
    output wire        ternary_exciton_out,
    output reg         led_erlang_hb,
    output wire        led_fault_trip
);
    localparam [7:0]  CODE_NEUTRAL     = 8'd121;
    localparam [7:0]  CODE_DARK_VORTEX = 8'd251;
    localparam [7:0]  CODE_SAFETY_TRIP = 8'd253;

    localparam signed [15:0] MAX_VSWR_LSB  = 16'sd6554;
    localparam signed [15:0] MAX_TOWNS_LSB = 16'sd16384;
    localparam signed [15:0] MIN_HELM_LSB  = 16'sd19660;
    localparam signed [15:0] MAX_TEMP_LSB  = 16'sd26869;

    localparam [3:0]
        ST_BOOT_RST   = 4'd0,
        ST_ACQUIRE    = 4'd1,
        ST_CONV_SIG   = 4'd2,
        ST_WAIT_SIG   = 4'd3,
        ST_SHIFT_SIG  = 4'd4,
        ST_SAFETY_CHK = 4'd5,
        ST_DARK_MUTE  = 4'd6,
        ST_CONV_DARK  = 4'd7,
        ST_WAIT_DARK  = 4'd8,
        ST_SHIFT_DARK = 4'd9,
        ST_PROC_LOAD  = 4'd10,
        ST_PROC_WAIT  = 4'd11,
        ST_WINDING    = 4'd12,
        ST_VBST_PACK  = 4'd13,
        ST_CRC_STEP   = 4'd14,
        ST_COMMIT     = 4'd15;

    reg [3:0]  state;
    reg [11:0] timer_cnt;
    reg        shift_is_dark;
    reg [7:0]  sclk_phase;
    reg [15:0] lane_shreg [0:3];

    reg signed [15:0] adc_sig_mem [0:31];
    reg signed [15:0] adc_dc_mem  [0:25];

    reg [2:0] alpha_sync;
    reg [1:0] esp_fc_sync;
    reg [1:0] busy_sync_0, busy_sync_1, busy_sync_2, busy_sync_3;
    wire      alpha_rise = (alpha_sync[2:1] == 2'b01);
    wire      all_adc_ready = ~(busy_sync_0[1] | busy_sync_1[1] |
                                busy_sync_2[1] | busy_sync_3[1]);

    reg [3:0]  decay_cnt;
    reg [7:0]  us_prescale;
    reg [31:0] us_accum;
    reg [31:0] erlang_dt_latched;
    reg [31:0] frame_seq;

    reg safety_trip_latched;
    assign hw_rf_hv_en         = rst_n & esp_fc_sync[1] & (~safety_trip_latched);
    assign led_fault_trip      = ~hw_rf_hv_en;
    assign ternary_exciton_out = hw_rf_hv_en ? exciton_fpga_in : 1'b0;

    wire core_busy   = (state != ST_ACQUIRE);
    assign leg1_busy = core_busy | (leg1_start & ~leg1_done);
    assign leg2_busy = core_busy | (leg2_start & ~leg2_done);

    reg [3:0]         ch_idx;
    reg               cordic_start;
    wire              cordic_done;
    reg signed [15:0] cur_i_cal, cur_q_cal;
    wire signed [15:0] cordic_phi_q13;

    reg [12:0]        trit_pos_mask;
    reg [12:0]        trit_neg_mask;
    reg [12:0]        vortex_null_mask;
    reg signed [15:0] phi_arr_q13 [0:12];
    reg signed [23:0] winding_acc;
    reg [7:0]         kintsugi_code;

    cbj_cordic_atan2_q13 u_cordic (
        .clk     (clk_sys),
        .rst_n   (rst_n),
        .start   (cordic_start),
        .i_in    (cur_i_cal),
        .q_in    (cur_q_cal),
        .done    (cordic_done),
        .phi_q13 (cordic_phi_q13)
    );

    wire signed [31:0] cur_mag_sq   = (cur_i_cal * cur_i_cal) + (cur_q_cal * cur_q_cal);
    wire signed [16:0] abs_q_raw    = (cur_q_cal < 16'sd0) ? -{cur_q_cal[15], cur_q_cal}
                                                           :  {cur_q_cal[15], cur_q_cal};
    wire signed [31:0] cur_abs_q_14 = {{15{1'b0}}, abs_q_raw} <<< 14;
    wire signed [31:0] cur_sqrt3_i  = cur_i_cal * 32'sd28378;

    reg [7:0] work_buf [0:63];
    reg [7:0] snap_buf [0:63];
    reg       pending_snap_copy;
    reg [5:0] byte_idx;
    reg [15:0] crc_reg;

    wire [5:0] spi1_addr, spi2_addr;
    wire       spi1_act,  spi2_act;

    cbj_spi_slave_64b u_spi_leg1 (
        .clk       (clk_sys),
        .rst_n     (rst_n),
        .spi_sck   (spi1_sck),
        .spi_cs_n  (spi1_cs_n),
        .spi_mosi  (spi1_mosi),
        .spi_miso  (spi1_miso),
        .cs_active (spi1_act),
        .rd_addr   (spi1_addr),
        .rd_data   (snap_buf[spi1_addr])
    );

    cbj_spi_slave_64b u_spi_leg2 (
        .clk       (clk_sys),
        .rst_n     (rst_n),
        .spi_sck   (spi2_sck),
        .spi_cs_n  (spi2_cs_n),
        .spi_mosi  (spi2_mosi),
        .spi_miso  (spi2_miso),
        .cs_active (spi2_act),
        .rd_addr   (spi2_addr),
        .rd_data   (snap_buf[spi2_addr])
    );

    function [15:0] crc16_ccitt_step(input [15:0] crc_in, input [7:0] d);
        reg [15:0] c;
        integer b;
        begin
            c = crc_in ^ {d, 8'h00};
            for (b = 0; b < 8; b = b + 1) begin
                if (c[15])
                    c = (c << 1) ^ 16'h1021;
                else
                    c = (c << 1);
            end
            crc16_ccitt_step = c;
        end
    endfunction

    function [7:0] pack_vbst5(
        input [12:0] pos_m,
        input [12:0] neg_m,
        input [3:0]  base_idx
    );
        reg [7:0] acc;
        reg [7:0] u_trit;
        integer m;
        reg [7:0] w3;
        begin
            acc = 8'd0;
            w3  = 8'd1;
            for (m = 0; m < 5; m = m + 1) begin
                u_trit = 8'd1;
                if ((base_idx + m) < 13) begin
                    if (pos_m[base_idx + m])
                        u_trit = 8'd2;
                    else if (neg_m[base_idx + m])
                        u_trit = 8'd0;
                end
                acc = acc + (u_trit * w3);
                w3  = w3 * 8'd3;
            end
            pack_vbst5 = acc;
        end
    endfunction

    integer idx;
    wire [3:0] next_ch_w = (ch_idx == 4'd12) ? 4'd0 : (ch_idx + 4'd1);
    wire signed [17:0] raw_dphi = {{2{phi_arr_q13[next_ch_w][15]}}, phi_arr_q13[next_ch_w]} -
                                  {{2{phi_arr_q13[ch_idx][15]}},    phi_arr_q13[ch_idx]};
    wire signed [17:0] wrap_dphi = (raw_dphi >  18'sd25736) ? (raw_dphi - 18'sd51472) :
                                   (raw_dphi < -18'sd25736) ? (raw_dphi + 18'sd51472) :
                                                               raw_dphi;
    wire signed [15:0] wind_q8_w = winding_acc[20:5];

    wire [2:0] adc_word_off = sclk_phase[7:5];
    wire [15:0] next_sh0 = {lane_shreg[0][14:0], adc_miso[0]};
    wire [15:0] next_sh1 = {lane_shreg[1][14:0], adc_miso[1]};
    wire [15:0] next_sh2 = {lane_shreg[2][14:0], adc_miso[2]};
    wire [15:0] next_sh3 = {lane_shreg[3][14:0], adc_miso[3]};

    always @(posedge clk_sys or negedge rst_n) begin
        if (!rst_n) begin
            state               <= ST_BOOT_RST;
            timer_cnt           <= 12'd0;
            shift_is_dark       <= 1'b0;
            sclk_phase          <= 8'd0;
            adc_convst          <= 1'b1;
            adc_sclk            <= 1'b1;
            adc_reset           <= 1'b1;
            adc_cs_n            <= 4'b1111;
            shil_env_gate       <= 1'b0;
            dark_cal_sw_ctl     <= 1'b0;
            erlang_strobe_a     <= 1'b0;
            erlang_strobe_c     <= 1'b0;
            led_erlang_hb       <= 1'b0;
            alpha_sync          <= 3'b000;
            esp_fc_sync         <= 2'b11;
            busy_sync_0         <= 2'b00;
            busy_sync_1         <= 2'b00;
            busy_sync_2         <= 2'b00;
            busy_sync_3         <= 2'b00;
            decay_cnt           <= 4'd0;
            us_prescale         <= 8'd0;
            us_accum            <= 32'd0;
            erlang_dt_latched   <= 32'd0;
            frame_seq           <= 32'd0;
            safety_trip_latched <= 1'b0;
            ch_idx              <= 4'd0;
            cordic_start        <= 1'b0;
            cur_i_cal           <= 16'sd0;
            cur_q_cal           <= 16'sd0;
            trit_pos_mask       <= 13'd0;
            trit_neg_mask       <= 13'd0;
            vortex_null_mask    <= 13'd0;
            winding_acc         <= 24'sd0;
            kintsugi_code       <= CODE_NEUTRAL;
            pending_snap_copy   <= 1'b0;
            byte_idx            <= 6'd0;
            crc_reg             <= 16'hFFFF;
            for (idx = 0; idx < 4; idx = idx + 1)  lane_shreg[idx]  <= 16'd0;
            for (idx = 0; idx < 32; idx = idx + 1) adc_sig_mem[idx] <= 16'sd0;
            for (idx = 0; idx < 26; idx = idx + 1) adc_dc_mem[idx]  <= 16'sd0;
            for (idx = 0; idx < 13; idx = idx + 1) phi_arr_q13[idx] <= 16'sd0;
            for (idx = 0; idx < 64; idx = idx + 1) begin
                work_buf[idx] <= 8'h00;
                snap_buf[idx] <= 8'h00;
            end
        end else begin
            alpha_sync      <= {alpha_sync[1:0],  alpha_schmitt};
            esp_fc_sync     <= {esp_fc_sync[0],   esp_fail_closed_n};
            busy_sync_0     <= {busy_sync_0[0],   adc_busy[0]};
            busy_sync_1     <= {busy_sync_1[0],   adc_busy[1]};
            busy_sync_2     <= {busy_sync_2[0],   adc_busy[2]};
            busy_sync_3     <= {busy_sync_3[0],   adc_busy[3]};
            cordic_start    <= 1'b0;
            erlang_strobe_a <= 1'b0;
            erlang_strobe_c <= 1'b0;

            if (!esp_fc_sync[1])
                safety_trip_latched <= 1'b1;

            if (us_prescale == (CLK_HZ_DIV_1US - 1)) begin
                us_prescale <= 8'd0;
                us_accum    <= us_accum + 32'd1;
            end else begin
                us_prescale <= us_prescale + 8'd1;
            end

            if (pending_snap_copy && !spi1_act && !spi2_act) begin
                for (idx = 0; idx < 64; idx = idx + 1)
                    snap_buf[idx] <= work_buf[idx];
                pending_snap_copy <= 1'b0;
                erlang_strobe_a   <= 1'b1;
                erlang_strobe_c   <= 1'b1;
                led_erlang_hb     <= ~led_erlang_hb;
            end

            case (state)
                ST_BOOT_RST: begin
                    if (timer_cnt == 12'd8) begin
                        adc_reset     <= 1'b0;
                        shil_env_gate <= 1'b1;
                        timer_cnt     <= 12'd0;
                        state         <= ST_ACQUIRE;
                    end else begin
                        timer_cnt <= timer_cnt + 12'd1;
                    end
                end

                ST_ACQUIRE: begin
                    shil_env_gate   <= ~safety_trip_latched;
                    dark_cal_sw_ctl <= 1'b0;
                    if (alpha_rise) begin
                        if (decay_cnt == 4'd12) begin
                            decay_cnt         <= 4'd0;
                            erlang_dt_latched <= us_accum;
                            us_accum          <= 32'd0;
                            us_prescale       <= 8'd0;
                            frame_seq         <= frame_seq + 32'd1;
                            adc_convst        <= 1'b0;
                            timer_cnt         <= 12'd0;
                            state             <= ST_CONV_SIG;
                        end else begin
                            decay_cnt <= decay_cnt + 4'd1;
                        end
                    end
                end

                ST_CONV_SIG: begin
                    adc_convst <= 1'b1;
                    if (timer_cnt == 12'd4) begin
                        timer_cnt <= 12'd0;
                        state     <= ST_WAIT_SIG;
                    end else begin
                        timer_cnt <= timer_cnt + 12'd1;
                    end
                end

                ST_WAIT_SIG: begin
                    if (all_adc_ready) begin
                        adc_cs_n      <= 4'b0000;
                        adc_sclk      <= 1'b1;
                        sclk_phase    <= 8'd0;
                        shift_is_dark <= 1'b0;
                        state         <= ST_SHIFT_SIG;
                    end
                end

                ST_SHIFT_SIG, ST_SHIFT_DARK: begin
                    if (sclk_phase[0] == 1'b0) begin
                        adc_sclk   <= 1'b0;
                        sclk_phase <= sclk_phase + 8'd1;
                    end else begin
                        adc_sclk      <= 1'b1;
                        lane_shreg[0] <= next_sh0;
                        lane_shreg[1] <= next_sh1;
                        lane_shreg[2] <= next_sh2;
                        lane_shreg[3] <= next_sh3;

                        if (sclk_phase[4:1] == 4'd15) begin
                            if (!shift_is_dark) begin
                                adc_sig_mem[{2'd0, adc_word_off}] <= $signed(next_sh0);
                                adc_sig_mem[{2'd1, adc_word_off}] <= $signed(next_sh1);
                                adc_sig_mem[{2'd2, adc_word_off}] <= $signed(next_sh2);
                                adc_sig_mem[{2'd3, adc_word_off}] <= $signed(next_sh3);
                            end else begin
                                adc_dc_mem[{2'd0, adc_word_off}]  <= $signed(next_sh0);
                                adc_dc_mem[{2'd1, adc_word_off}]  <= $signed(next_sh1);
                                adc_dc_mem[{2'd2, adc_word_off}]  <= $signed(next_sh2);
                                if (adc_word_off < 3'd2)
                                    adc_dc_mem[{2'd3, adc_word_off}] <= $signed(next_sh3);
                            end
                        end

                        if (sclk_phase == 8'd255) begin
                            adc_cs_n <= 4'b1111;
                            state    <= shift_is_dark ? ST_PROC_LOAD : ST_SAFETY_CHK;
                            ch_idx   <= 4'd0;
                            trit_pos_mask    <= 13'd0;
                            trit_neg_mask    <= 13'd0;
                            vortex_null_mask <= 13'd0;
                        end else begin
                            sclk_phase <= sclk_phase + 8'd1;
                        end
                    end
                end

                ST_SAFETY_CHK: begin
                    if ((adc_sig_mem[27] > MAX_VSWR_LSB)  ||
                        (adc_sig_mem[29] > MAX_TOWNS_LSB) ||
                        (adc_sig_mem[30] < MIN_HELM_LSB)  ||
                        (adc_sig_mem[31] > MAX_TEMP_LSB)  ||
                        safety_trip_latched) begin
                        safety_trip_latched <= 1'b1;
                        kintsugi_code       <= CODE_SAFETY_TRIP;
                    end else begin
                        kintsugi_code <= CODE_NEUTRAL;
                    end
                    shil_env_gate   <= 1'b0;
                    dark_cal_sw_ctl <= 1'b1;
                    timer_cnt       <= 12'd0;
                    state           <= ST_DARK_MUTE;
                end

                ST_DARK_MUTE: begin
                    if (timer_cnt == (DARK_MUTE_CYC - 1)) begin
                        adc_convst <= 1'b0;
                        timer_cnt  <= 12'd0;
                        state      <= ST_CONV_DARK;
                    end else begin
                        timer_cnt <= timer_cnt + 12'd1;
                    end
                end

                ST_CONV_DARK: begin
                    adc_convst <= 1'b1;
                    if (timer_cnt == 12'd4) begin
                        timer_cnt <= 12'd0;
                        state     <= ST_WAIT_DARK;
                    end else begin
                        timer_cnt <= timer_cnt + 12'd1;
                    end
                end

                ST_WAIT_DARK: begin
                    if (all_adc_ready) begin
                        dark_cal_sw_ctl <= 1'b0;
                        shil_env_gate   <= ~safety_trip_latched;
                        adc_cs_n        <= 4'b0000;
                        adc_sclk        <= 1'b1;
                        sclk_phase      <= 8'd0;
                        shift_is_dark   <= 1'b1;
                        state           <= ST_SHIFT_DARK;
                    end
                end

                ST_PROC_LOAD: begin
                    cur_i_cal    <= adc_sig_mem[{ch_idx, 1'b0}] - adc_dc_mem[{ch_idx, 1'b0}];
                    cur_q_cal    <= adc_sig_mem[{ch_idx, 1'b1}] - adc_dc_mem[{ch_idx, 1'b1}];
                    cordic_start <= 1'b1;
                    state        <= ST_PROC_WAIT;
                end

                ST_PROC_WAIT: begin
                    if (cordic_done) begin
                        phi_arr_q13[ch_idx] <= cordic_phi_q13;

                        if (cur_mag_sq < R_NULL_SQ) begin
                            vortex_null_mask[ch_idx] <= 1'b1;
                            if (!safety_trip_latched)
                                kintsugi_code <= CODE_DARK_VORTEX;
                        end else if ((cur_i_cal > 16'sd0) && (cur_abs_q_14 < cur_sqrt3_i)) begin
                            // Basin 0
                        end else if (cur_q_cal >= 16'sd0) begin
                            trit_pos_mask[ch_idx] <= 1'b1;
                        end else begin
                            trit_neg_mask[ch_idx] <= 1'b1;
                        end

                        if (ch_idx == 4'd12) begin
                            ch_idx      <= 4'd0;
                            winding_acc <= 24'sd0;
                            state       <= ST_WINDING;
                        end else begin
                            ch_idx <= ch_idx + 4'd1;
                            state  <= ST_PROC_LOAD;
                        end
                    end
                end

                ST_WINDING: begin
                    winding_acc <= winding_acc + {{6{wrap_dphi[17]}}, wrap_dphi};
                    if (ch_idx == 4'd12) begin
                        state <= ST_VBST_PACK;
                    end else begin
                        ch_idx <= ch_idx + 4'd1;
                    end
                end

                ST_VBST_PACK: begin
                    work_buf[0]  <= 8'h31; work_buf[1]  <= 8'h4A;
                    work_buf[2]  <= 8'h42; work_buf[3]  <= 8'h43;
                    work_buf[4]  <= frame_seq[7:0];
                    work_buf[5]  <= frame_seq[15:8];
                    work_buf[6]  <= frame_seq[23:16];
                    work_buf[7]  <= frame_seq[31:24];
                    work_buf[8]  <= erlang_dt_latched[7:0];
                    work_buf[9]  <= erlang_dt_latched[15:8];
                    work_buf[10] <= erlang_dt_latched[23:16];
                    work_buf[11] <= erlang_dt_latched[31:24];
                    work_buf[12] <= trit_pos_mask[7:0];
                    work_buf[13] <= {3'b000, trit_pos_mask[12:8]};
                    work_buf[14] <= trit_neg_mask[7:0];
                    work_buf[15] <= {3'b000, trit_neg_mask[12:8]};
                    work_buf[16] <= vortex_null_mask[7:0];
                    work_buf[17] <= {3'b000, vortex_null_mask[12:8]};
                    work_buf[18] <= wind_q8_w[7:0];
                    work_buf[19] <= wind_q8_w[15:8];

                    for (idx = 0; idx < 6; idx = idx + 1) begin
                        work_buf[20 + 2*idx] <= adc_sig_mem[26 + idx][7:0];
                        work_buf[21 + 2*idx] <= adc_sig_mem[26 + idx][15:8];
                    end
                    for (idx = 0; idx < 13; idx = idx + 1) begin
                        work_buf[32 + 2*idx] <= phi_arr_q13[idx][7:0];
                        work_buf[33 + 2*idx] <= phi_arr_q13[idx][15:8];
                    end

                    work_buf[58] <= pack_vbst5(trit_pos_mask, trit_neg_mask, 4'd0);
                    work_buf[59] <= pack_vbst5(trit_pos_mask, trit_neg_mask, 4'd5);
                    work_buf[60] <= pack_vbst5(trit_pos_mask, trit_neg_mask, 4'd10);
                    work_buf[61] <= safety_trip_latched ? CODE_SAFETY_TRIP : kintsugi_code;

                    byte_idx <= 6'd0;
                    crc_reg  <= 16'hFFFF;
                    state    <= ST_CRC_STEP;
                end

                ST_CRC_STEP: begin
                    crc_reg <= crc16_ccitt_step(crc_reg, work_buf[byte_idx]);
                    if (byte_idx == 6'd61) begin
                        state <= ST_COMMIT;
                    end else begin
                        byte_idx <= byte_idx + 6'd1;
                    end
                end

                ST_COMMIT: begin
                    work_buf[62] <= crc_reg[7:0];
                    work_buf[63] <= crc_reg[15:8];
                    if (!spi1_act && !spi2_act) begin
                        for (idx = 0; idx < 62; idx = idx + 1)
                            snap_buf[idx] <= work_buf[idx];
                        snap_buf[62]    <= crc_reg[7:0];
                        snap_buf[63]    <= crc_reg[15:8];
                        erlang_strobe_a <= 1'b1;
                        erlang_strobe_c <= 1'b1;
                        led_erlang_hb   <= ~led_erlang_hb;
                    end else begin
                        pending_snap_copy <= 1'b1;
                    end
                    state <= ST_ACQUIRE;
                end
            endcase
        end
    end
endmodule
`default_nettype wire
