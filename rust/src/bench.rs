use std::os::unix::net::UnixStream;
use std::io::{self, Read, Write};
use std::time::Instant;

pub fn run_mtp_benchmark(socket_path: &str, iterations: usize) -> io::Result<()> {
    let mut stream = UnixStream::connect(socket_path)?;
    let payload = [0x55u8; 64];
    let mut ack_buf = [0u8; 4];
    let mut latencies = Vec::with_capacity(iterations);

    for _ in 0..iterations {
        let mut frame = Vec::with_capacity(11 + payload.len());
        frame.extend_from_slice(&0x44414E44u32.to_ne_bytes());
        frame.push(1);
        frame.extend_from_slice(&0x0101u32.to_ne_bytes());
        frame.extend_from_slice(&(payload.len() as u16).to_ne_bytes());
        frame.extend_from_slice(&payload);

        let t0 = Instant::now();
        stream.write_all(&frame)?;
        stream.read_exact(&mut ack_buf)?;
        latencies.push(t0.elapsed().as_secs_f64() * 1_000_000.0);
    }

    latencies.sort_by(|a, b| a.partial_cmp(b).unwrap());
    let p50 = latencies[iterations / 2];
    let p99 = latencies[(iterations * 99) / 100];
    let mean = latencies.iter().sum::<f64>() / (iterations as f64);
    println!("[RUST BENCH] Mean: {:.2} us | p50: {:.2} us | p99: {:.2} us (Ceiling <= 35.90 us)", mean, p50, p99);
    Ok(())
}
