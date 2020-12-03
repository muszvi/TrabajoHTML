# Trabajo de Informática Industrial

El código crea un servidor web en la placa NodeMCU, donde conectándose a la página web que crea, se le puede encender y apagar el LED que lleva integrado.

Para poder hacer esto hay que seguir los siguientes pasos:

1. Descargar la carpeta webserver.
2. Una vez descargada, abrir el archivo .config, y donde aparece

```c
const char* ssid = "NAME";
const char* password = "PASSWORD";
```

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; hay que hambiar _NAME_ por el nombre del WiFi, y _PASSWORD_ por la contraseña.

3. Hay que descargarse la librería _NTPClient_.
4. Abrir webserver.ino y enviar el código a la placa,
5. Una vez que se haya subido, abrir el Serial en _115200 baud_.
6. En el serial se avisará una vez que se haya conectado a la red WiFi y devolverá la dirección IP en la que está el servidor web alojado.
7. Para poder entrar en la página web desde cualquier navegador, hay que introducir la dirección IP (por ejemplo 192.168.0.1) seguida de :8080/. 
   Así, la página web debe quedar como _192.168.0.1:8080/_.
