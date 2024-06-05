# Como usar el server (para pruebas)

```bash
$ echo -en "INICIO\nLOGIN 123456789 64639997e21870375e5d65cd454421dee2185f2bde1e863f28ad2c1cb5a3de3e851366d41f2836a5a9a4c510eb2e36fe8ef0226ae8a0c325731029fa6e53\nREQUEST INSURANCE_STATUS\nLOGOUT\nQUIT" | nc 127.0.0.1 8080
```
La opción -n es que no haya trailing newline character
La opción -e es que si haya interpretación de secuencias de escape, como \n
nc es netcat, en modo cliente y leyendo de stdin

# Descripción de protocolo

.**Session example**

```.
INICIO // 0 - 1
   OK/NOT_OK: Server is busy or under maintenance
LOGIN <USER> <HASH> // 1 - 1
   OK/NOT_OK: Incorrect username or password
REQUEST USER_DATA <id>    // 2 - n
   OK/NOT_OK
REQUEST INSURANCE_STATUS <id>    // 2 - n
   OK/NOT_OK
   ...
REQUEST LAB_LIST    // 2 - n
   OK/NOT_OK
REQUEST LAB_RESULT <labId>    // 2 - n
   OK/NOT_OK
   ...

LOGOUT // 2 - 1 
   OK
QUIT // 1 - 1, 2 - 1 (+LOGOUT)
   OK
   ...
```

Description:
