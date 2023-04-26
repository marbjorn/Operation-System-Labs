fun main() {
    val factory = SMFactory()
    val terminator = factory.HireSpaceMarine(UnitName.Terminator)
    val assault = factory.HireSpaceMarine(UnitName.AssaultSpaceMarine)
    val sergeant = factory.HireSpaceMarine(UnitName.Sergeant)
    val commonMarine = factory.HireSpaceMarine(UnitName.Marine)

    terminator!!.getInfo()
    assault!!.getInfo()
    sergeant!!.getInfo()
    commonMarine!!.getInfo()

    val terminatorDmg = terminator.attackWithAllWeapon()
    println("Terminator: $terminatorDmg")

    val commonMarineDmg = commonMarine.attackWithAllWeapon()
    println("Common space marice: $commonMarineDmg")
}