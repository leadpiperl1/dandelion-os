use std::os::unix::net::UnixStream;
use std::io::{self, Write};

pub struct DandelionIpcBridge {
    stream: UnixStream,
}

impl DandelionIpcBridge {
    pub fn connect(socket_path: &str) -> io::Result<Self> {
        let stream = UnixStream::connect(socket_path)?;
        Ok(Self { stream })
    }

    pub fn send_telemetry(&mut self, node_id: u32, triadic_tag: i8, payload: &[u8]) -> io::Result<()> {
        let mut frame = Vec::with_capacity(11 + payload.len());
        frame.extend_from_slice(&0x44414E44u32.to_ne_bytes());
        frame.push(triadic_tag as u8);
        frame.extend_from_slice(&node_id.to_ne_bytes());
        frame.extend_from_slice(&(payload.len() as u16).to_ne_bytes());
        frame.extend_from_slice(payload);
        self.stream.write_all(&frame)?;
        self.stream.flush()
    }
}
