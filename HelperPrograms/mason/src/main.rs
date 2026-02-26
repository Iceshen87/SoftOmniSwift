use crate::arguments::arguments::Arguments;
use std::process::exit;

mod arguments;
mod processing;

fn main() {
    let arguments = Arguments::parse_from_standard_in();
    if arguments.is_none() {
        exit(1);
    }

    let arguments = arguments.unwrap();
    if arguments.is_help() {
        println!(

        );
        exit(0);
    }

    if arguments.is_license() {
        println!(

        );
        exit(0);
    }

    if arguments.is_version() {
        let major: &str = env!("CARGO_PKG_VERSION_MAJOR");
        let minor: &str = env!("CARGO_PKG_VERSION_MINOR");
        let patch: &str = env!("CARGO_PKG_VERSION_PATCH");

        let mut target = String::new();
        if cfg!(windows) {
            target.push_str("Windows");
        } else if cfg!(target_os = "android") {
            target.push_str("Android");
        } else if cfg!(target_os = "ios") {
            target.push_str("iOS");
        } else if cfg!(target_arch = "aarch64-apple-darwin") {
            target.push_str("MacOS Darwin");
        } else if cfg!(target_os = "macos") {
            target.push_str("MacOS Intel");
        } else if cfg!(target_family = "wasm") {
            target.push_str("WebAsaembly");
        } else if cfg!(target_os = "linux") {
            target.push_str("Linux")
        } else if cfg!(unix) {
            target.push_str("other Unix");
        } else {
            target.push_str("unknown OS");
        }

        println!(
            "SoftOmni Swift Libraries Meson Sub Directory File Generator Helper Program Version\n\
             {}.{}.{}\n\
             Compiled for {}", major, minor, patch, target
        );
        exit(0);
    }

    processing::processing::process(&arguments);
    exit(0);
}
