## Describtion
This educational task was completed for a colloquium in order to demonstrate the application of design patterns in practice. 
Written in the Kotlin programming language.

## Context
*For the Emperor! - General battle cry*

The project simulates the attack phase for a space marine in the [Warhammer 40.000](https://ru.wikipedia.org/wiki/Warhammer_40,000) tabletop game.
![](https://d1w82usnq70pt2.cloudfront.net/wp-content/uploads/2019/07/Artums_Ultras.jpg "Artum's_Ultras")

Let's imagine a situation. An enemy [tyranid](https://warhammer40k.fandom.com/wiki/Tyranids) is running towards your space marine, and you need to calculate how much damage the marine can deal to the enemy. 
The code implements 4 types of space marines:
 1. [**Tactical space marine**](https://warhammer40k.fandom.com/wiki/Tactical_Marine). The main strategic force of your army. Usually equipped with a bolter, plasma pistol, or chainsword.
 2. [**Terminator**](https://warhammer40k.fandom.com/wiki/Terminators). Veteran space marines, heavy infantry. Usually equipped with power fists and bolters.
 3. [**Assault space marine**](https://warhammer40k.fandom.com/wiki/Assault_Marine). A close support Firstborn Space Marine who is equipped with weapons that are specialized towards melee combat. Equipped with a pistol, chainsword, or bolter.
 4. [**Sergeant**](https://warhammer40k.fandom.com/wiki/Sergeant_%28Space_Marine%29). Squad leader, usually equipped with a pistol and chainsword.

***The Factory pattern** was used to organize the declaration of a space marine of a particular type.*

So, let's go back to calculating damage. To determine whether a unit hit an enemy with a weapon, a certain number of dice are rolled and the number of "successful" rolls is calculated.

As can be seen from the description above, each space marine can be equipped uniquely but within their own type. There are 4 types of weapons:
 1. [**Chainsword**](https://warhammer40k.fandom.com/wiki/Chainsword). Close combat weapon. Make 3 rolls of d3 for the attack, sum the values - total damage.
 2. [**Bolter**](https://warhammer40k.fandom.com/wiki/Bolter). Ranged weapon. Roll 6 d6 dice with rerolls of 1, successful hit only on 3+. Then roll d3 dice for each successful hit, sum the values - total damage.
 3. [**Power fist**](https://warhammer40k.fandom.com/wiki/Power_Fist). Close combat weapon. Roll 3 d6 dice, maximum value - total damage.
 4. [**Plasma pistol**](https://warhammer40k.fandom.com/wiki/Plasma_Pistol). Ranged plasma weapon. Roll 3 d3 dice, hit on 2+. Then roll d6 dice for each successful hit, sum the values - total damage.

***The Strategy pattern** was applied to calculate the attack with each weapon.*

