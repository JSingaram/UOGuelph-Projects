package rogue;

/**
* Signals that a user's move is invalid and was not carried out.
*
*/
public class InvalidMoveException extends Exception {


    /**
    * Constructs an InvalidMoveException with "That is an invalid move." as its error detail message.
    *
    */
    public InvalidMoveException() {
        super("That is an invalid move.");
    }


    /**
    * Constructs an InvalidMoveException with input String as its error detail message.
    * @param str String containing error message
    *
    */
    public InvalidMoveException(String str) {
        super(str);
    }
}
