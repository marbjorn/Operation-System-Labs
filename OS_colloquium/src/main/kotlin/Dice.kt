import kotlin.random.Random

class Dice (val edgesNum : Int) {
    fun roll() : Int {
        return Random.nextInt(1, edgesNum+1)
    }
}