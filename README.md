<h2>🚀 Building a CLI Chat Application with Cross-Connected Server-Client Model 💻📡</h2>

![{B12F1EAF-5256-463B-9031-73FDCEC60CBB}](https://github.com/user-attachments/assets/94466663-f8ac-4bf8-af82-1962be392bed)

<p>I recently got some time and decided to upgrade my basic TCP chat app which I made during my internship. So here I am, have developed a CLI-based chat application using TCP sockets that follows a server-client model with an interesting twist—cross-connection between the server and client. This was a fascinating project that deepened my understanding of socket programming, bidirectional communication, file transfer mechanisms, threading, testing and debugging techniques!</p>

<h2>🔍 Key Features of the Chat Application</h2>
<p>✅ Menu-Based Approach – Designed an intuitive menu system for better user interaction and control over functionalities.</p>
<p>✅ Bidirectional Communication – Implemented a cross-connection mechanism where the server and client each act as both a server and a client, ensuring full-duplex communication.</p>
<p>✅ File Sharing – Clients can send and receive files to / from the server, along with retrieving file information from the server location.</p>
<p>✅ Heartbeat Mechanism – Implemented a heartbeat logic to periodically check the responsiveness of the client-server connection and handle disconnections gracefully.</p>
<p>✅ Threading – Used multithreading to manage different tasks simultaneously, ensuring smooth performance and responsiveness.</p>

<h2>🔍 How the Chat Application Works</h2>
<p>•	The application consists of one server and one client, with the server supporting one active client at a time.</p>
<p>•	Unlike traditional chat applications where a client sends data to a server and waits for a response, this chat application ensures true bidirectional communication using a cross-connection approach:</p>
<p>  o	The server application contains both a server and a client component.</p>
<p>  o	The client application also contains both a server and a client component.</p>
<p>  o	The server's server connects with the client's client, and the client's server connects with the server's client—forming a fully duplexed communication channel.</p>
<p>•	The application also supports file transfer, allowing users to send and receive files to/from the server. Additionally, clients can retrieve file information from the server location.</p>

<h2>🔧 Tools & Techniques Used</h2>
<p>🛠 Valgrind – Performed memory leak checks, ensuring optimal memory management (validated with a Valgrind report showing no memory leaks 🚀).</p>
<p>🛠 GDB – Used GNU Debugger (GDB) to debug and analyze the application efficiently.</p>
<p>🛠 Git & GitHub – Learned to manage code versions, maintain a GitHub repo, and perform regular commits for structured development and upgrades.</p>
<p>🛠 Threads – Implemented multithreading to handle concurrent tasks seamlessly.</p>
<p>🛠 System programming – Used various Linux system APIs to integrate seamless chat app and file transfer.</p>

<h2>🔥 Key Learnings from This Project</h2>
<p>✅ Advanced TCP Socket Programming – Deepened my knowledge of TCP sockets, connection handling, and bidirectional communication.</p>
<p>✅ Version Control Best Practices – Gained experience in structured Git workflows, managing branches, and maintaining a clean commit history.</p>
<p>✅ Bidirectional Communication – Explored how cross-connection between server and client enables seamless data exchange.</p>
<p>✅ Memory Management & Debugging – Used Valgrind for memory leak detection and GDB for debugging to enhance code stability.</p>
<p>✅Multithreading & Synchronization – Learned to efficiently manage multiple threads to handle chat and file transfers concurrently.</p>
<p>✅ File Transfer over Sockets – Implemented sending and receiving files over TCP efficiently.</p>
<p>✅ Concurrency Management – Designed a structured way to handle client-server interactions while managing connections.</p>
<p>✅ Network Reliability – Implemented heartbeat logic to detect and handle unresponsive connections.</p>
<p>✅ Extensive Testing efforts – Tested the application for every possible error that could be reproducible.</p>

