# AgeOfFiuba

Juego Taller de Programacion I
Segundo cuatrimestre de 2015 

Integrantes:
Kaoru Martin Heanna; 91891
Diego Juan Martins Forgan; 92290
Nicolas Mariano Fernandez Lema; 93410
Dario Leandro Miñones; 86644
Federico Longhi; 93336


Sistema Operativo: Ubuntu

Para poder usarlo el proyecto compilado tiene que estar en una carpeta junto con las carpetas img y yaml-files
Se puede cambiar la configuracion del juego modificando el archivo configuracionMapa.yaml
NO MODIFICAR configuracion.yaml NI BORRAR LA CARPETA img YA QUE SON LOS DATOS DEFAULT DE LA APP

Para poder compilar el proyecto se necesitan las siguientes librerias:

- yaml-cpp
- SDL2
- SDL_image
- SDL_mixer
- SDL_ttf

Para instalar estas librerias en Linux ejecutar lo siguiente:

sudo apt-get install libsdl2-dev
sudo apt-get install libsdl2-image-dev
sudo apt-get install libsdl2-ttf-dev
sudo apt-get install libsdl2-mixer-dev

sudo apt-get install libyaml-cpp-dev
sudo apt-get install libboost-dev

Para compilar el programa nosotros usamos Eclipse pero se puede compilar con los siguientes comandos:

g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Views/Drawable.d" -MT"Views/Drawable.d" -o "Views/Drawable.o" "../Views/Drawable.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Views/EntityView.d" -MT"Views/EntityView.d" -o "Views/EntityView.o" "../Views/EntityView.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Views/MapView.d" -MT"Views/MapView.d" -o "Views/MapView.o" "../Views/MapView.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Views/MobileView.d" -MT"Views/MobileView.d" -o "Views/MobileView.o" "../Views/MobileView.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Views/Renderer.d" -MT"Views/Renderer.d" -o "Views/Renderer.o" "../Views/Renderer.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Views/Sprite.d" -MT"Views/Sprite.d" -o "Views/Sprite.o" "../Views/Sprite.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Views/View.d" -MT"Views/View.d" -o "Views/View.o" "../Views/View.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Utils/Log.d" -MT"Utils/Log.d" -o "Utils/Log.o" "../Utils/Log.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Models/Entity.d" -MT"Models/Entity.d" -o "Models/Entity.o" "../Models/Entity.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Models/Escenario.d" -MT"Models/Escenario.d" -o "Models/Escenario.o" "../Models/Escenario.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Models/Map.d" -MT"Models/Map.d" -o "Models/Map.o" "../Models/Map.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Models/MobileModel.d" -MT"Models/MobileModel.d" -o "Models/MobileModel.o" "../Models/MobileModel.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Models/TileSet.d" -MT"Models/TileSet.d" -o "Models/TileSet.o" "../Models/TileSet.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Controllers/GameController.d" -MT"Controllers/GameController.d" -o "Controllers/GameController.o" "../Controllers/GameController.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Configuration/ConfiguracionConfig.d" -MT"Configuration/ConfiguracionConfig.d" -o "Configuration/ConfiguracionConfig.o" "../Configuration/ConfiguracionConfig.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Configuration/EntidadConfig.d" -MT"Configuration/EntidadConfig.d" -o "Configuration/EntidadConfig.o" "../Configuration/EntidadConfig.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Configuration/EscenarioConfig.d" -MT"Configuration/EscenarioConfig.d" -o "Configuration/EscenarioConfig.o" "../Configuration/EscenarioConfig.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Configuration/GameConfiguration.d" -MT"Configuration/GameConfiguration.d" -o "Configuration/GameConfiguration.o" "../Configuration/GameConfiguration.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Configuration/PantallaConfig.d" -MT"Configuration/PantallaConfig.d" -o "Configuration/PantallaConfig.o" "../Configuration/PantallaConfig.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Configuration/TipoConfig.d" -MT"Configuration/TipoConfig.d" -o "Configuration/TipoConfig.o" "../Configuration/TipoConfig.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"AgeOfFiuba.d" -MT"AgeOfFiuba.d" -o "AgeOfFiuba.o" "../AgeOfFiuba.cpp"
g++ -L/usr/include/SDL2 -o "AgeOfFiuba"  ./Views/Drawable.o ./Views/EntityView.o ./Views/MapView.o ./Views/MobileView.o ./Views/Renderer.o ./Views/Sprite.o ./Views/View.o  ./Utils/Log.o  ./Models/Entity.o ./Models/Escenario.o ./Models/Map.o ./Models/MobileModel.o ./Models/TileSet.o  ./Controllers/GameController.o  ./Configuration/ConfiguracionConfig.o ./Configuration/EntidadConfig.o ./Configuration/EscenarioConfig.o ./Configuration/GameConfiguration.o ./Configuration/PantallaConfig.o ./Configuration/TipoConfig.o  ./AgeOfFiuba.o   -lSDL2 -lyaml-cpp -lSDL2_mixer -lSDL2_ttf -lSDL2_image