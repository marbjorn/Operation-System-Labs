import kotlin.random.Random

class Dice (val edgesNum : Int) {
    fun roll() : Int {
        return Random.nextInt(1, edgesNum+1)
    }

}

fun Dice.rollDices(dice : Dice, amount : Int) : ArrayList<Int> {
    val rolled = ArrayList<Int>(amount)
    for (i in rolled.indices) {
        rolled[i] = dice.roll()
    }
    return rolled
}