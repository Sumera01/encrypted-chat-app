# 🔒 Encrypted Chat Application

A secure terminal-based chat application featuring ROT4 encryption and real-time messaging with performance metrics.

![C](https://img.shields.io/badge/C-Programming-blue)
![License](https://img.shields.io/badge/License-MIT-green)
![Platform](https://img.shields.io/badge/Platform-Linux-lightgrey)
![Network](https://img.shields.io/badge/Network-TCP_Sockets-orange)

## ✨ Features

- 🔒 **ROT4 Encryption** - Basic character substitution encryption for learning purposes
- ⏱️ **Performance Metrics** - Real-time message transmission timing display
- 💬 **Real-time Chat** - Simultaneous sending and receiving capabilities
- 🎨 **NCurses UI** - Elegant terminal-based user interface
- 🔌 **TCP Socket Communication** - Reliable network connectivity
- 🚀 **Multi-threaded** - Non-blocking message reception

## 📋 Prerequisites

- GCC compiler
- NCurses library
- pthread library

### Ubuntu/Debian Installation:
```bash
sudo apt-get update
sudo apt-get install build-essential libncurses5-dev
```

### CentOS/RHEL Installation:
```bash
sudo yum groupinstall "Development Tools"
sudo yum install ncurses-devel
```

## 🛠️ Installation

1. **Clone the repository**
```bash
git clone https://github.com/yourusername/encrypted-chat-app.git
cd encrypted-chat-app
```

2. **Compile the application**
```bash
make
```

## 🚀 Usage

### Local Testing (Two terminals on same machine):

1. **Start Peer 1 in Terminal 1:**
```bash
./bin/peer1
```
When prompted, enter: `127.0.0.1`

2. **Start Peer 2 in Terminal 2:**
```bash
./bin/peer2
```
When prompted, enter: `127.0.0.1`

### Network Testing (Different machines):

1. **On Machine 1 - Run Peer 1:**
```bash
./bin/peer1
```
Enter the IP address of Machine 2

2. **On Machine 2 - Run Peer 2:**
```bash
./bin/peer2
```
Enter the IP address of Machine 1

### 💬 Chat Commands:
- Type your message and press **Enter** to send
- Type `/quit` to exit the application gracefully

## 🏗️ Project Structure

```
encrypted-chat-app/
├── src/               # Source code files
│   ├── peer1.c        # Peer 1 implementation
│   └── peer2.c        # Peer 2 implementation
├── bin/               # Compiled binaries (created after make)
├── include/           # Header files
├── docs/              # Documentation
├── Makefile           # Build configuration
├── LICENSE           # MIT License
└── README.md         # This file
```

## 🔧 Technical Details

### Encryption Scheme
- **ROT4 Cipher**: Caesar cipher with 4-position shift
- Alphabetic characters are shifted by 4 positions
- Non-alphabetic characters remain unchanged
- Example: 'A' → 'E', 'z' → 'd'

### Network Configuration
- **Peer 1**: Listens on port 5555, connects to port 5556
- **Peer 2**: Listens on port 5556, connects to port 5555
- Automatic connection retry mechanism

### Performance Metrics
- Real-time display of message encryption/decryption time
- Network transmission latency measurement
- Displayed in milliseconds for each message

## 🐛 Troubleshooting

### Common Issues:

1. **"Connection failed" error:**
   - Ensure both peers are running
   - Check firewall settings
   - Verify IP addresses are correct

2. **"ncurses.h not found" during compilation:**
   - Install ncurses development package: `sudo apt-get install libncurses5-dev`

3. **Permission denied errors:**
   - Ensure ports 5555-5556 are not blocked
   - Run as user with network permissions

## 🤝 Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 👨‍💻 Author

Your Name  
- GitHub: [@Sumera01](https://github.com/Sumera01)

---

**Note**: This project is for educational purposes and uses basic ROT4 encryption. For production use, consider implementing stronger encryption algorithms like AES.
