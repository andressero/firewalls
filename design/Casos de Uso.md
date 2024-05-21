# Casos de Uso

## Caso de uso 1:
Descripción: Un usuario quiere conocer su estado de asegurado.
Actor: Todo usuario posible del sistema, incluyendo pacientes, doctores y laboratoristas.
Stakeholders: Administrador del sistema.
Precondiciones: El usuario esta registrado en el sistema.
Postcondiciones: -
Flujo básico:
El usuario inicia sesión en el sistema.
Se dirige al menú de verificación de derechos desde el menú principal.
Escribe su identificador en el campo donde se le solicita.
El sistema le muestra la ventana de usuario asegurado.
Caminos alternativos:
1. El usuario no esta asegurado. Se le muestra la ventana de usuario no asegurado.

## Caso de uso 2:
Descripción: Un paciente quiere consultar su examen de laboratorio más reciente.
Actor: Usuario paciente.
Stakeholders: Usuario laboratorista, usuario doctor.
Precondiciones: El usuario esta registrado en el sistema y se le realizó un examen de laboratorio.
Postcondiciones: -
Flujo básico:
El usuario inicia sesión en el sistema.
Se dirige al menú de examenes de laboratorio desde el menú principal.
Elige entre los examenes disponibles el que esta buscando.
El sistema le muestra la ventaja del examen.
Caminos alternativos:
1. Aún no esta disponible el examen que esta buscando, a razón de que el laboratorista no lo ha terminado. El usuario vuelve a intentar en otro momento.
2. El usuario elige el examen equivocado. El usuario puede volver de nuevo al menú de los examenes y elegir otra vez un examen.
