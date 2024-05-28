# Descripción de protocolo

.**Session example**

```.
INICIO
   OK/NOT_OK: Server is busy or under maintenance
LOGIN <USER> PASS
   OK/NOT_OK: Incorrect password
REQUEST INSURANCE_STATUS
   OK/NOT_OK
   ...
REQUEST LAB_RESULTS
   OK/NOT_OK
   ...
QUIT
   OK
   ...
```

Description:
