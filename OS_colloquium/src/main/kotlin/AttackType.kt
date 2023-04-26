interface WarGearStrategy {
    fun attack() : Int
    fun hitting(tries : Int, dice : Dice, toHit : Int, reRollingValue : Int = 0) : Int {
        println("--------HIT PHASE--------")
        var curRoll : Int
        var hitCounter = 0
        var reRollCounter = 0
        for (i in 1..tries) {
            curRoll = dice.roll()
            print("\tRolling the dice $i : $curRoll")
            if (curRoll >= toHit) {
                println(" - hit!")
                hitCounter++
            }
            else {
                if (curRoll <= reRollingValue) reRollCounter++
                println(" - miss!")
            }

        }

        if (reRollCounter == 0) return hitCounter

        println("Re-rolling $reRollCounter ones...")
        for (i in 1..reRollCounter) {
            curRoll = dice.roll()
            if (curRoll >= toHit) {
                println("\t$curRoll - hit!")
                hitCounter++
            }
            else {
                println("\t$curRoll - miss!")
            }
        }
        return hitCounter
    }

    fun calculateDmg(hits : Int, dice: Dice) : Int {
        println("--------DAMAGE PHASE--------")
        var dmgCounter = 0
        println("Rolling $hits dices d${dice.edgesNum}... ")
        for (i in 1..hits) {
            dmgCounter += dice.roll()
        }
        return dmgCounter
    }
}

class ChainswordStrategy() : WarGearStrategy {
    override fun attack() : Int {
        val d3 = Dice(3)
        println("Attack with chainsword! Rolling 3 dices d3...")
        val dmg = hitting(3,d3, 1)
        println("Total damage is ${dmg}")
        return dmg
    }
}

class BolterStrategy() : WarGearStrategy {
    override fun attack() : Int {
        val d6 = Dice(6)
        val d3 = Dice(3)
        println("Attack with bolter! Rolling 6 dices d6, hit only on 3+, re-rolling 1")
        val hits = hitting(6, d6, 3, 1)
        val dmgCounter = calculateDmg(hits, d3)
        println("Total damage is $dmgCounter")
        return dmgCounter
    }
}

class PlasmaPistolStrategy() : WarGearStrategy {
    override fun attack(): Int {
        val d3 = Dice(3)
        val d6 = Dice(6)
        println("Attack with plasma pistol! Rolling 3 dices d3, hit on 2+")
        val hits = hitting(3,d3,2)
        val dmg = calculateDmg(hits, d6)
        println("Total damage is $dmg")
        return dmg
    }
}

class PowerFistStrategy() : WarGearStrategy {
    override fun attack() : Int {
        val d6 = Dice(6)
        var curDmg : Int;
        println("Attack with power fist! Rolling 3 dices d6, choose the greatest values")
        var maxDamageValue = 0;
        for (i in 1..3) {
            curDmg = d6.roll()
            println("Rolling the dice $i : $curDmg")
            if (maxDamageValue < curDmg) maxDamageValue = curDmg
        }
        println("Total damage is $maxDamageValue")
        return maxDamageValue
    }
}