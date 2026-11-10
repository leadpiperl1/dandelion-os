pub mod net;
pub mod ipc;
pub mod bench;

fn main() -> std::io::Result<()> {
    let socket_path = "/tmp/dandelion_mesh.sock";
    if std::path::Path::new(socket_path).exists() {
        bench::run_mtp_benchmark(socket_path, 10_000)?;
    } else {
        println!("[RUST] Dandelion Data Plane ready (socket {} inactive; skipping live IPC loop).", socket_path);
    }
    Ok(())
}
