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
