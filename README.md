# Proyecto Final - CGIHC

Proyecto final de la materia del Laboratorio de Computación Gráfica e Interacción Humano-Computadora del grupo 03 del semestre 2019-1 de la Facultad de Ingeniería, UNAM.

## Descripción del Proyecto

Este proyecto es una aplicación gráfica en 3D desarrollada en C++ utilizando **OpenGL**. Presenta una reconstrucción interactiva de la fachada del **Edificio Q (Luis G Valdés Vallejo)** y el interior del **Laboratorio de Computación Gráfica** de la Facultad de Ingeniería.

El entorno fue construido usando modelado geométrico con base en primitivas, modelado jerárquico y mapeo de texturas para representar con precisión el laboratorio.

## Características Principales

- **Renderizado 3D**: Modelado detallado del salón, que incluye pisos, techos, paredes, ventanas, puertas, pizarrones, escritorios, computadoras (CPUs, monitores y teclados) y sillas.
- **Sistema de Texturas**: Uso extensivo de texturas (ladrillos, concreto, escudos, madera, metales y plásticos) cargadas con `SOIL2` y `stb_image`.
- **Cámara Sintética**: Sistema de cámara libre en primera persona para navegar por todo el entorno usando teclado y ratón.
- **Modelos Externos**: Soporte para la carga de modelos 3D complejos (`.obj`) mediante la librería `Assimp`.

## Controles de Navegación y Animaciones

El proyecto cuenta con múltiples componentes dinámicos con animaciones interactivas que pueden ser activadas por el usuario:

### Movimiento (Cámara)
- `W` : Mover hacia adelante
- `S` : Mover hacia atrás
- `A` : Mover a la izquierda
- `D` : Mover a la derecha
- `Ratón` : Controlar la dirección de la vista (apuntar)

### Animaciones Interactivas
- `P` : Abrir / Cerrar las puertas corredizas del laboratorio.
- `C` : Animar las computadoras (movimiento articulado de los monitores).
- `E` : Activar el elevador (subir / bajar de piso).
- `Q` : Activar el aire acondicionado (rotación de aspas/ventilación).

### Sistema
- `ESC` : Salir de la aplicación.

## Tecnologías y Dependencias Usadas

- C++
- **OpenGL**
- **GLFW** y **GLEW** (Manejo de ventanas y contexto)
- **GLM** (OpenGL Mathematics)
- **SOIL2 / stb_image** (Carga y procesamiento de texturas)
- **Assimp** (Open Asset Import Library, carga de modelos `.obj`)
