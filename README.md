# Aero-Army
## Download 
* [Play Store Link](https://play.google.com/store/apps/details?id=com.invoksion.Aeroforces)
<img width="945" alt="image" src="https://github.com/elemcraft/Aero-Army/assets/48006644/9c71fe0c-2dbf-4675-88e9-42f1e4e1374d">

## Environment
* Unreal Engine 4.27
* C++11

## Features
* Implement a sophisticated control system utilizing projection matrix to ensure a consistent one-to-one mapping between the player's finger movement and character displacement, even across devices with varying resolutions and pixel densities.
* Utilize inheritance principle to common functionalities shared among various enemy types. For example, by implementing the applying damage functionality in the superclass inherited by all enemy classes, this strategy minimized code redundancy and enhanced scalability as it enables seamless integration of new enemy types with minimal effort.
* Utilize dynamic polymorphism to implement varying damage calculations when a player projectile collides with different enemy instances. Through this approach, distinct enemy types receive customized damage amounts upon collision.
* Develop a robust countdown system that functions without requiring an internet connection. This is achieved by storing the timestamp when each operation is initiated, allowing precise tracking of time elapsed even in offline mode. 


## Lessons learned
1. Familiarize myself with C++ pointer and reference
2. Implement object oriented programming techniques, Polymorphism, Inheritance, Encapsulation, and Abstraction.
3. Unreal Engine is not an ideal tool for moible game development
4. Always document project process
5. Version control is a life-saver
