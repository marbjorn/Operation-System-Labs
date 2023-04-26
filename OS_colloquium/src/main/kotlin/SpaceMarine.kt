enum class Weapon {CHAINSWORD, BOLTER, PLASMAPISTOL, POWERFIST}
enum class UnitType {FastAttack, HeavySupport, Troops, HQ}
enum class UnitName {Terminator, AssaultSpaceMarine, Marine, Sergeant}

class SMFactory {
    fun HireSpaceMarine(kind : UnitName) : SpaceMarine? {
        when(kind) {
            UnitName.Terminator -> return Terminator()
            UnitName.Sergeant -> return Sergeant()
            UnitName.AssaultSpaceMarine -> return AssaultSpaceMarine()
            UnitName.Marine -> return Marine()
            else -> return null
        }
    }
}

interface SpaceMarine {
    val warGear : ArrayList<Weapon>
    val points : Int
    val type : UnitType
    fun getInfo() {
        println("---------UNIT INFO----------")
        when (type) {
            UnitType.HeavySupport -> println("Heavy support unit, $points. Tactics: cover a strategic force")
            UnitType.FastAttack -> println("Fast attack unit, $points. Tactics: engage in close combat with the enemy")
            UnitType.Troops -> println("Space marine troop unit, $points. Tactics: main strategic force")
            UnitType.HQ -> println("HQ (headquarter), $points. Tactics: leads the squad into battle")
        }
    }

    fun attackWithAllWeapon() : Int {
        var dmgSum = 0
        for (i in warGear) {
            dmgSum += when(i) {
                Weapon.BOLTER -> BolterStrategy().attack()
                Weapon.POWERFIST -> PowerFistStrategy().attack()
                Weapon.CHAINSWORD -> ChainswordStrategy().attack()
                Weapon.PLASMAPISTOL -> PlasmaPistolStrategy().attack()
            }
        }
        return dmgSum
    }
}

class Terminator : SpaceMarine {
    override val warGear: ArrayList<Weapon> = arrayListOf(Weapon.POWERFIST, Weapon.POWERFIST)
    override val points = 55
    override val type = UnitType.HeavySupport
}

class AssaultSpaceMarine : SpaceMarine {
    override val warGear: ArrayList<Weapon> = arrayListOf(Weapon.CHAINSWORD, Weapon.BOLTER)
    override val points = 45
    override val type = UnitType.FastAttack
}

class Sergeant : SpaceMarine {
    override val warGear: ArrayList<Weapon> = arrayListOf(Weapon.PLASMAPISTOL, Weapon.POWERFIST)
    override val points = 90
    override val type = UnitType.HQ
}

class Marine : SpaceMarine {
    override val warGear: ArrayList<Weapon> = arrayListOf(Weapon.BOLTER)
    override val points = 35
    override val type = UnitType.Troops
}