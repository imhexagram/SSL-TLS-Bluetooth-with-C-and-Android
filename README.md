### Simple Exemple Socket Communication with C

socket_client.c, socket_server.c

### Simple Exemple SSL/TLS Socket Communication with C

ssl_client.c, ssl_server.c

### Bluetooth Communication between C server and Android Client

bluetooth.c, SecuBluetooth.zip

### SSL/TLS Bluetooth Communication between C and Android

ssl_bluetooth.c, TODO-Android part

### cmd

```console

$ openssl genpkey -algorithm RSA -out server_key.key

$ openssl req -new -key server_key.key -out server_csr.csr

$ openssl x509 -req -days 365 -in server_csr.csr -signkey server_key.key -out server_cert.crt

$ gcc test.c -o test -lbluetooth -lssl -lcrypto

```

```console

openssl genrsa -des3 -out caroot.key 2048
openssl req -new -x509 -days 3650 -key caroot.key -out caroot.crt
openssl x509 -in caroot.crt -text -noout
openssl genrsa -des3 -out server.key 2048
openssl req -new -key server.key -out server.csr
openssl x509 -req -in server.csr -out server.crt -sha1 -CA caroot.crt -CAkey caroot.key -CAcreateserial -days 3650
openssl x509 -in server.crt -text -noout
openssl rsa -in server.key -out server.key.insecure

openssl genrsa -des3 -out client.key 2048
openssl req -new -key client.key -out client.csr
openssl x509 -req -in client.csr -out client.crt -sha1 -CA caroot.crt -CAkey caroot.key -CAcreateserial -days 3650
openssl x509 -in client.crt -text -noout
openssl rsa -in client.key -out client.key.insecure

openssl pkcs12 -export  -in client.crt -inkey client.key -out client.p12
keytool -importkeystore -destkeystore client.jks -srckeystore client.p12  -srcstoretype pkcs12
openssl pkcs12 -export  -in caroot.crt -inkey caroot.key -out caroot.p12
keytool -importkeystore -destkeystore caroot.jks -srckeystore caroot.p12  -srcstoretype pkcs12
```

### Reference

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
