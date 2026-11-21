use std::io::{Read, Write};
use std::net::TcpListener;

const HEALTH_ADDR: &str = "0.0.0.0:9445";
const HEALTH_RESPONSE: &[u8] =
    b"HTTP/1.1 200 OK\r\nContent-Length: 2\r\nConnection: close\r\n\r\nok";

fn main() -> std::io::Result<()> {
    let listener = TcpListener::bind(HEALTH_ADDR)?;
    println!("data-plane health endpoint on {HEALTH_ADDR}");

    // Answers every request with 200 ok, which is all the Kubernetes probes need.
    for mut stream in listener.incoming().flatten() {
        let mut buf = [0u8; 1024];
        let _ = stream.read(&mut buf);
        let _ = stream.write_all(HEALTH_RESPONSE);
    }
    Ok(())
}
