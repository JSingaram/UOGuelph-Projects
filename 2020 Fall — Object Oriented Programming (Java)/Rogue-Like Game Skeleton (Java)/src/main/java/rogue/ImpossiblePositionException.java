package rogue;

/**
* Signals that a user's move is invalid and was not carried out.
*
*/
public class ImpossiblePositionException extends Exception {


    /**
    * Constructs an InvalidMoveException with "That is an invalid move." as its error detail message.
    *
    */
    public ImpossiblePositionException() {
        super("This position is not possible.");
    }


    /**
    * Constructs an InvalidMoveException with input String as its error detail message.
    * @param str String containing error message
    *
    */
    public ImpossiblePositionException(String str) {
        super(str);
    }
}
