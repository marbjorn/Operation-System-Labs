fun main() {
    val factory = SMFactory()
    val terminator = factory.hireSpaceMarine(UnitName.Terminator)
    val assault = factory.hireSpaceMarine(UnitName.AssaultSpaceMarine)
    val sergeant = factory.hireSpaceMarine(UnitName.Sergeant)
    val commonMarine = factory.hireSpaceMarine(UnitName.Marine)

    terminator!!.getInfo()
    assault!!.getInfo()
    sergeant!!.getInfo()
    commonMarine!!.getInfo()

    val terminatorDmg = terminator.attackWithAllWeapon()
    println("Terminator: $terminatorDmg")

    val commonMarineDmg = commonMarine.attackWithAllWeapon()
    println("Common space marice: $commonMarineDmg")
}