# Test info
* Password Rules: ^(?=.*[a-z])(?=.*[A-Z])(?=.*\d)(?=.*[\#\$\%\^\&\+\=\!\_\-\(\)\=\?\*\.\<\>]).{8,64}$
* Using argom2id: parallelism factor: 2, mem.cost: 16, iters: 20, hashLen: 64
* rightsState:   0 (Yes), 1(No)

## Old ones
* Password Rules: ^(?=.*[a-z])(?=.*[A-Z])(?=.*\d)(?=.*[\#\$\%\^\&\+\=\!\_\-\(\)\=\?\*\.\<\>]).{8,16}$
* Using argom2d: parallelism factor: 2, mem. cost: 16, iter: 2, hashLen : 32
* rightsState: 0 (Yes), 1(No)

## User examples (editar)
```
user0: user, passwordHash, salt, userType, rightsState (0 || 1)

user1: 123456789, passHash: db9aef989cc25fdea3a7f0187296a1ebcfbbe4dec674291a8eec55fb0e4c676f (password: p4Ssword1_), salt: K3BwBQMb2w8xnxAG, userType: P, rightsState: 0

user2: L41234, passHash: 441d3f3a35ac351500ce87f6fa85714dd325f798760091f6587b686de43694ec (password: M3dico_Lab!), salt: FQmhXai1D3ONAOoa, userType: L, rightsState: 0

user3: 112341234, passHash: b4f2e6188faded17ee5be4c33abec994a5e31082e3c62ab91098dde72a9122c0 (password: 0perating_Sys775), salt: KlhDzMuyzNxnFKwn, P, rightsState: 1
``
## Random Salt
https://www.geeksforgeeks.org/how-to-create-a-random-alpha-numeric-string-in-cpp/
## Argon
https://github.com/khovratovich/Argon2/tree/master