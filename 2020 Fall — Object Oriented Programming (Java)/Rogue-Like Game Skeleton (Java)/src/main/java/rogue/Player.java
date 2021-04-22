package rogue;

import java.util.ArrayList;
import java.awt.Point;

/**
* The player character.
*/
public class Player {
    private String name;
    private Point xyLocation;
    private Room currentRoom;
    private ArrayList<Item> inventory = new ArrayList<Item>();
    // static final Integer X = 4;
    // static final Integer Y = 1;


    /**
    * Initializes a newly created Player with default values: name = "The Adventurer"
    * and Point = a new point with coordinates (2, 2).
    *
    */
    public Player() {
        this("The Adventurer");
    }


    /**
    * Initializes a newly created Player with input name and a default starting
    * position of (2, 2).
    * @param newName The name of the Player
    *
    */
    public Player(String newName) {
        Point p = new Point(1, 1);
        setXyLocation(p);
        setName(newName);
    }


    /**
    * Returns the name of the player as a String.
    * @return The name of the player
    *
    */
    public String getName() {
        return name;
    }


    /**
    * Sets the name of the player as a String.
    * @param newName The new name of the player
    *
    */
    public void setName(String newName) {
        name = newName;
    }


    /**
    * Returns the location of the player as a Point.
    * @return The location of the player
    *
    */
    public Point getXyLocation() {
        return xyLocation;
    }


    /**
    * Sets the location of the player as a Point.
    * @param newXyLocation The new location of the player
    *
    */
    public void setXyLocation(Point newXyLocation) {
        xyLocation = newXyLocation;
    }


    /**
    * Returns the current room the player is in as a Room.
    * @return The current room the player is in
    *
    */
    public Room getCurrentRoom() {
        return currentRoom;
    }


    /**
    * Sets the current room the player is in as a Room.
    * @param newRoom The new current room the player is in
    *
    */
    public void setCurrentRoom(Room newRoom) {
        currentRoom = newRoom;
        newRoom.setIsPlayerInRoom(true);
    }


    /**
    * Returns the player's inventory as an ArrayList.
    * @return The player's inventory
    *
    */
    public ArrayList<Item> getInventory() {
        return inventory;
    }


    /**
    * Adds an item to the player's inventory.
    * @param newItem The item to add to inventory
    *
    */
    public void addItem(Item newItem) {
        inventory.add(newItem);
    }
}
