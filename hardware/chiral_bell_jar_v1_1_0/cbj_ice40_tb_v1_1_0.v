`timescale 1ns / 1ps
`default_nettype none
/*
 * cbj_ice40_tb_v1_1_0.v — Deterministic Icarus Verilog Self-Checking Testbench
 * Author: Justin D. Grimm (ORCID: 0009-0000-5299-0243) | Version: v1.1.0
 */

module cbj_ice40_tb;
    reg        clk_sys         = 1'b0;
    reg        rst_n           = 1'b0;
    reg        alpha_schmitt   = 1'b0;
    wire       adc_convst, adc_sclk, adc_reset;
    wire [3:0] adc_cs_n;
    reg  [3:0] adc_busy        = 4'b0000;
    reg  [3:0] adc_miso        = 4'b0000;
    wire       shil_env_gate, dark_cal_sw_ctl, hw_rf_hv_en;
    reg        esp_fail_closed_n = 1'b1;

    reg        spi1_sck        = 1'b0;
    reg        spi1_cs_n       = 1'b1;
    reg        spi1_mosi       = 1'b0;
    wire       spi1_miso, erlang_strobe_a, leg1_busy;
    reg        leg1_start      = 1'b0, leg1_done = 1'b0;

    reg        spi2_sck        = 1'b0;
    reg        spi2_cs_n       = 1'b1;
    reg        spi2_mosi       = 1'b0;
    wire       spi2_miso, erlang_strobe_c, leg2_busy;
    reg        leg2_start      = 1'b0, leg2_done = 1'b0;

    reg        exciton_fpga_in = 1'b1;
    wire       ternary_exciton_out, led_erlang_hb, led_fault_trip;

    always #12.5 clk_sys = ~clk_sys;

    cbj_ice40_top #(
        .CLK_HZ_DIV_1US (40),
        .DARK_MUTE_CYC  (40)
    ) dut (
        .clk_sys             (clk_sys),
        .rst_n               (rst_n),
        .alpha_schmitt       (alpha_schmitt),
        .adc_convst          (adc_convst),
        .adc_sclk            (adc_sclk),
        .adc_reset           (adc_reset),
        .adc_cs_n            (adc_cs_n),
        .adc_busy            (adc_busy),
        .adc_miso            (adc_miso),
        .shil_env_gate       (shil_env_gate),
        .dark_cal_sw_ctl     (dark_cal_sw_ctl),
        .esp_fail_closed_n   (esp_fail_closed_n),
        .hw_rf_hv_en         (hw_rf_hv_en),
        .spi1_sck            (spi1_sck),
        .spi1_cs_n           (spi1_cs_n),
        .spi1_mosi           (spi1_mosi),
        .spi1_miso           (spi1_miso),
        .erlang_strobe_a     (erlang_strobe_a),
        .leg1_start          (leg1_start),
        .leg1_busy           (leg1_busy),
        .leg1_done           (leg1_done),
        .spi2_sck            (spi2_sck),
        .spi2_cs_n           (spi2_cs_n),
        .spi2_mosi           (spi2_mosi),
        .spi2_miso           (spi2_miso),
        .erlang_strobe_c     (erlang_strobe_c),
        .leg2_start          (leg2_start),
        .leg2_busy           (leg2_busy),
        .leg2_done           (leg2_done),
        .exciton_fpga_in     (exciton_fpga_in),
        .ternary_exciton_out (ternary_exciton_out),
        .led_erlang_hb       (led_erlang_hb),
        .led_fault_trip      (led_fault_trip)
    );

    reg signed [15:0] tb_sig_true [0:31];
    reg signed [15:0] tb_dc_off   [0:31];
    reg signed [15:0] tb_latched  [0:31];
    reg [6:0]         tb_bit_ptr;

    always @(negedge adc_convst) begin
        adc_busy <= 4'b1111;
        for (integer c = 0; c < 32; c = c + 1) begin
            if (dark_cal_sw_ctl)
                tb_latched[c] = tb_dc_off[c];
            else
                tb_latched[c] = tb_sig_true[c] + tb_dc_off[c];
        end
        #150 adc_busy <= 4'b0000;
    end

    always @(negedge adc_cs_n[0]) begin
        tb_bit_ptr = 7'd0;
    end

    always @(negedge adc_sclk) begin
        if (adc_cs_n == 4'b0000) begin
            adc_miso[0] <= tb_latched[0  + tb_bit_ptr[6:4]][4'd15 - tb_bit_ptr[3:0]];
            adc_miso[1] <= tb_latched[8  + tb_bit_ptr[6:4]][4'd15 - tb_bit_ptr[3:0]];
            adc_miso[2] <= tb_latched[16 + tb_bit_ptr[6:4]][4'd15 - tb_bit_ptr[3:0]];
            adc_miso[3] <= tb_latched[24 + tb_bit_ptr[6:4]][4'd15 - tb_bit_ptr[3:0]];
            tb_bit_ptr  <= tb_bit_ptr + 7'd1;
        end
    end

    reg [7:0] rx_frame [0:63];
    task spi_read_64b;
        integer b, bit_i;
        begin
            spi1_cs_n = 1'b0;
            #200;
            for (b = 0; b < 64; b = b + 1) begin
                for (bit_i = 7; bit_i >= 0; bit_i - 1) begin end
                for (bit_i = 7; bit_i >= 0; bit_i = bit_i - 1) begin
                    #100 spi1_sck = 1'b1;
                    rx_frame[b][bit_i] = spi1_miso;
                    #100 spi1_sck = 1'b0;
                end
            end
            #200 spi1_cs_n = 1'b1;
        end
    endtask

    task fire_13_alphas;
        integer a;
        begin
            for (a = 0; a < 13; a = a + 1) begin
                #100 alpha_schmitt = 1'b1;
                #100 alpha_schmitt = 1'b0;
            end
        end
    endtask

    function [15:0] tb_crc16(input integer len);
        reg [15:0] crc;
        integer i, b;
        begin
            crc = 16'hFFFF;
            for (i = 0; i < len; i = i + 1) begin
                crc = crc ^ {rx_frame[i], 8'h00};
                for (b = 0; b < 8; b = b + 1)
                    crc = crc[15] ? ((crc << 1) ^ 16'h1021) : (crc << 1);
            end
            tb_crc16 = crc;
        end
    endfunction

    real ang;
    reg [31:0] rx_magic;
    reg [15:0] rx_crc, exp_crc;
    reg signed [15:0] rx_wind;

    initial begin
        for (integer k = 0; k < 13; k = k + 1) begin
            ang = -3.141592653589793 + (2.0 * 3.141592653589793 * k) / 13.0;
            tb_sig_true[2*k]     = $rtoi(8000.0 * $cos(ang));
            tb_sig_true[2*k + 1] = $rtoi(8000.0 * $sin(ang));
            tb_dc_off[2*k]       = 16'sd1150 - (k * 90);
            tb_dc_off[2*k + 1]   = -16'sd840 + (k * 65);
        end
        tb_sig_true[26] = 16'sd18000; tb_dc_off[26] = 16'sd0;
        tb_sig_true[27] = 16'sd1200;  tb_dc_off[27] = 16'sd0;
        tb_sig_true[28] = 16'sd14000; tb_dc_off[28] = 16'sd0;
        tb_sig_true[29] = 16'sd4200;  tb_dc_off[29] = 16'sd0;
        tb_sig_true[30] = 16'sd22938; tb_dc_off[30] = 16'sd0;
        tb_sig_true[31] = 16'sd15500; tb_dc_off[31] = 16'sd0;

        #100 rst_n = 1'b1;
        #500;

        fire_13_alphas();
        @(posedge erlang_strobe_a);
        #100 spi_read_64b();

        rx_magic = {rx_frame[3], rx_frame[2], rx_frame[1], rx_frame[0]};
        rx_crc   = {rx_frame[63], rx_frame[62]};
        exp_crc  = tb_crc16(62);
        rx_wind  = $signed({rx_frame[19], rx_frame[18]});

        if (rx_magic !== 32'h43424A31 || rx_crc !== exp_crc || rx_frame[61] !== 8'd121) begin
            $display("[FAIL] Frame 1 header/CRC/status mismatch: magic=%h crc=%h/%h code=%d",
                     rx_magic, rx_crc, exp_crc, rx_frame[61]);
            $finish(1);
        end
        if (rx_wind < 16'sd1605 || rx_wind > 16'sd1611 || hw_rf_hv_en !== 1'b1) begin
            $display("[FAIL] Frame 1 CORDIC winding=%d (expected ~1608) en=%b", rx_wind, hw_rf_hv_en);
            $finish(1);
        end
        $display("[PASS] RTL T1: 64B SPI Mode-0, Dark-Slot DC Null, Q3.13 CORDIC W=+1 (wind=%0d), CRC=%h",
                 rx_wind, rx_crc);

        tb_sig_true[27] = 16'sd6555;
        fire_13_alphas();
        @(posedge erlang_strobe_a);
        #100 spi_read_64b();

        if (hw_rf_hv_en !== 1'b0 || ternary_exciton_out !== 1'b0 || rx_frame[61] !== 8'd253) begin
            $display("[FAIL] Frame 2 safety trip failed: en=%b exc=%b code=%d",
                     hw_rf_hv_en, ternary_exciton_out, rx_frame[61]);
            $finish(1);
        end
        $display("[PASS] RTL T2: 0-Tick Hardware VSWR Interlock (hw_rf_hv_en=0, Code=253)");
        $finish(0);
    end
endmodule
`default_nettype wire
