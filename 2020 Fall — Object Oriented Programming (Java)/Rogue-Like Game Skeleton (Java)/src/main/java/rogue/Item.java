package rogue;

import java.awt.Point;

/**
 * A basic Item class; basic functionality for both consumables and equipment.
 */
public class Item  {
    private int id;
    private String name;
    private String type;
    private String description;
    private Point xyLocation;
    private Character displayChar;
    private Room currentRoom;
    private int currentRoomId;


    /**
    * Initializes a newly created Item with default values: ID = -1, name = "name",
    * type = "type", and Point = a new point with coordinates (0, 0).
    *
    */
    public Item() {
        this(-1, "name", "type", new Point(0, 0));
    }


    /**
    * Initializes a newly created Item with input values.
    * @param newId The ID of the item
    * @param newName The name of the item
    * @param newType The type of the item
    * @param newXYLocation The location of the item
    *
    */
    public Item(int newId, String newName, String newType, Point newXYLocation) {
        setId(newId);
        setName(newName);
        setType(newType);
        setXyLocation(newXYLocation);
        setDisplayCharacter('.');
    }


    /**
    * Returns the id of the item as an int.
    * @return The id of the item.
    *
    */
    public int getId() {
        return id;
    }


    /**
    * Sets the id of the item as an int.
    * @param newId The new id of the item.
    *
    */
    public void setId(int newId) {
        id = newId;
    }


    /**
    * Returns the id of the room the item is in item as an int.
    * @return The id of the room the item is in
    *
    */
    public int getCurrentRoomId() {
        return currentRoomId;
    }


    /**
    * Sets the id of the room the item is in item as an int.
    * @param newRoomId The new id of the room the item is in
    *
    */
    public void setCurrentRoomId(int newRoomId) {
        currentRoomId = newRoomId;
    }


    /**
    * Returns the name of the item as a String.
    * @return The name of the item.
    *
    */
    public String getName() {
        return name;
    }


    /**
    * Sets the name of the item as a String.
    * @param newName The new name of the item.
    *
    */
    public void setName(String newName) {
        name = newName;
    }


    /**
    * Returns the type of the item as a String.
    * @return The type of the item.
    *
    */
    public String getType() {
        return type;
    }


    /**
    * Sets the type of the item as a String.
    * @param newType The new type of the item.
    *
    */
    public void setType(String newType) {
        type = newType;
    }


    /**
    * Returns the display character of the item as a char.
    * @return The display character of the item.
    *
    */
    public Character getDisplayCharacter() {
        return displayChar;
    }


    /**
    * Sets the display character of the item as a char.
    * @param newDisplayCharacter The new display character of the item.
    *
    */
    public void setDisplayCharacter(Character newDisplayCharacter) {
        displayChar = newDisplayCharacter;
    }


    /**
    * Returns the description of the item as a String.
    * @return The description character of the item.
    *
    */
    public String getDescription() {
        return description;
    }


    /**
    * Sets the description of the item as a String.
    * @param newDescription The new description character of the item.
    *
    */
    public void setDescription(String newDescription) {
        description = newDescription;
    }


    /**
    * Returns the location of the item as a Point.
    * @return The location character of the item.
    *
    */
    public Point getXyLocation() {
        return xyLocation;
    }


    /**
    * Sets the location of the item as a Point.
    * @param newXyLocation The new location character of the item.
    *
    */
    public void setXyLocation(Point newXyLocation) {
        xyLocation = newXyLocation;
    }


    /**
    * Returns the room containing the item as a Room.
    * @return The room containing the item.
    *
    */
    public Room getCurrentRoom() {
        return currentRoom;
    }


    /**
    * Sets the room containing the item as a Room.
    * @param newCurrentRoom The new room containing the item.
    *
    */
    public void setCurrentRoom(Room newCurrentRoom) {
        currentRoom = newCurrentRoom;
    }
}
