The binary is a simple UDP client. It creates an UDP socket and
sends a message to the server on the IP `127.0.0.1`, port `7777`.

We can see that `strace ./mystery` does not reveal the whole message.

Se observă că rulând ''strace ./mystery'' nu vedem tot mesajul trimis
(vezi apelul ''sendto'').


The solution is:
 - Run `strace -s 64` (by default `strace` shows only 32 characters)
 - Connect with `netcat` on localhost, port 7777 `nc -ul 7777`

The message is `This is a long message, because I'm a 0xBADA55!`.
