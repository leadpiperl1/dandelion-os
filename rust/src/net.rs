use std::net::UdpSocket;
use std::io::Result;

pub struct DandelionTunnel {
    socket: UdpSocket,
    node_id: u32,
}

impl DandelionTunnel {
    pub fn bind(addr: &str, node_id: u32) -> Result<Self> {
        let socket = UdpSocket::bind(addr)?;
        socket.set_nonblocking(true)?;
        Ok(Self { socket, node_id })
    }

    pub fn send_packet(&self, dest: &str, payload: &[u8], triadic_tag: i8) -> Result<usize> {
        let mut packet = Vec::with_capacity(14 + payload.len());
        packet.extend_from_slice(&0x44414E44u32.to_ne_bytes());
        packet.push(0x01);
        packet.push(triadic_tag as u8);
        packet.extend_from_slice(&0x0001u16.to_ne_bytes());
        packet.extend_from_slice(&self.node_id.to_ne_bytes());
        packet.extend_from_slice(&0x00000002u32.to_ne_bytes());
        packet.extend_from_slice(&(payload.len() as u16).to_ne_bytes());
        packet.extend_from_slice(payload);
        self.socket.send_to(&packet, dest)
    }
}
