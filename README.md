Simple Exemple Socket Communication with C

socket_client.c, socket_server.c

------------------------------------------------------------

Simple Exemple SSL/TLS Socket Communication with C

ssl_client.c, ssl_server.c

------------------------------------------------------------

Bluetooth Communication between C server and Android Client

bluetooth.c, SecuBluetooth.zip

------------------------------------------------------------

cmd

openssl genpkey -algorithm RSA -out server_key.key

openssl req -new -key server_key.key -out server_csr.csr

openssl x509 -req -days 365 -in server_csr.csr -signkey server_key.key -out server_cert.crt

gcc test.c -o test -lbluetooth -lssl -lcrypto

-----------------------------------------------------------

Reference

Bluetooth

https://github.com/edisonslightbulbs/rfcomm

https://people.csail.mit.edu/albert/bluez-intro/c404.html

https://developer.android.com/develop/connectivity/bluetooth/connect-bluetooth-devices?hl=zh-cn
https://developer.android.com/guide/topics/connectivity/bluetooth?hl=zh-cn#java

https://blog.csdn.net/sevensummer7/article/details/121889347
https://blog.csdn.net/xanadw/article/details/124382034

https://blog.51cto.com/kenkao/5990078


https://stackoverflow.com/questions/70245463/java-lang-securityexception-need-android-permission-bluetooth-connect-permissio


SSL/TLS

https://wiki.openssl.org/index.php/Simple_TLS_Server
https://stackoverflow.com/questions/7698488/turn-a-simple-socket-into-an-ssl-socket
https://datatracker.ietf.org/doc/html/rfc5246
https://developer.android.com/privacy-and-security/security-ssl?hl=zh-cn
