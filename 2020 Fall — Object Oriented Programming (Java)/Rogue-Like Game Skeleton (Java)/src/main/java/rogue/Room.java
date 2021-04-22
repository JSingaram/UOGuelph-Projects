package rogue;

import java.util.ArrayList;
import java.util.HashMap;
import java.awt.Point;

/**
* A room within the dungeon - contains monsters, treasure,
* doors out, etc.
*/
public class Room  {
    private int width;
    private int height;
    private int id;
    private boolean inRoom;
    private ArrayList<Item> items = new ArrayList<Item>();
    private HashMap<String, Door> doors = new HashMap<String, Door>();
    private Player player = new Player();
    private String roomDescription;
    private Rogue rogue = new Rogue();

    private Character doorSymbol;
    private Character nsWallSymbol;
    private Character ewWallSymbol;
    private Character floorSymbol;
    private Character passageSymbol;
    private Character playerSymbol;


    /**
    * Initializes a newly created Room with default values: ID = -1, inRoom = false,
    * height = 1, width = 1, and player = null.
    *
    */
    public Room() {
        this(-1, false, 1, 1);
    }


    /**
    * Initializes a newly created Item with input values.
    * @param newId The ID of the room
    * @param newInRoom Whether or not the player is in the room
    * @param newHeight The height of the room
    * @param newWidth The width of the room
    *
    */
    public Room(int newId, boolean newInRoom, int newHeight, int newWidth) {
        setId(newId);
        inRoom = newInRoom;
        setHeight(newHeight);
        setWidth(newWidth);
    }


    /**
    * Sets the ASCII symbols used to represent the room.
    * @param newDoorSymbol The new symbol for doors in the room
    * @param nsWall The new symbol for north and south walls in the room
    * @param ewWall The new symbol for east and west walls in the room
    * @param floor The new symbol for unoccupied floors in the room
    * @param passage The new symbol for passages
    * @param newPlayer The new symbol for players in the room
    *
    */
    public void setSymbols(Character newDoorSymbol, Character nsWall, Character ewWall, Character floor,
                           Character passage, Character newPlayer) {
        doorSymbol = newDoorSymbol;
        nsWallSymbol = nsWall;
        ewWallSymbol = ewWall;
        floorSymbol = floor;
        passageSymbol = passage;
        playerSymbol = newPlayer;
    }


    /**
    * Returns the width of the room as an int.
    * @return The width of the room
    *
    */
    public int getWidth() {
        return width;
    }


    /**
    * Sets the width of the room as an int.
    * @param newWidth The new width of the room
    *
    */
    public void setWidth(int newWidth) {
        width = newWidth;
    }


    /**
    * Returns the height of the room as an int.
    * @return The height of the room
    *
    */
    public int getHeight() {
        return height;
    }


    /**
    * Sets the height of the room as an int.
    * @param newHeight The new height of the room
    *
    */
    public void setHeight(int newHeight) {
        height = newHeight;
    }


    /**
    * Returns the id of the room as an int.
    * @return The id of the room
    *
    */
    public int getId() {
        return id;
    }


    /**
    * Sets the id of the room as an int.
    * @param newId The new id of the room
    *
    */
    public void setId(int newId) {
        id = newId;
    }


    /**
    * Returns the items in the room as an ArrayList of Items.
    * @return The items in the room
    *
    */
    public ArrayList<Item> getRoomItems() {
        return items;
    }


    /**
    * Sets the items in the room as an ArrayList of Items.
    * @param newRoomItems The items in the room
    *
    */
    public void setRoomItems(ArrayList<Item> newRoomItems) {
        items = newRoomItems;
    }


    /**
    * Returns the player in the game as a Player.
    * @return The player in the game
    *
    */
    public Player getPlayer() {
        return player;
    }


    /**
    * Sets the player in the game as a Player.
    * @param newPlayer The player in the game
    *
    */
    public void setPlayer(Player newPlayer) {
        player = newPlayer;
        if (inRoom) {
            player.setCurrentRoom(this);
        }
    }


    /**
    * Returns the Door corresponding to input direction String.
    * @param direction The direction of the door that is desired (N/E/S/W)
    * @return The door found in that direction
    *
    */
    public Door getDoor(String direction) {
        return doors.get(direction);
    }


    /**
    * Returns the number of Doors in the room.
    * @return The number of Doors in the room
    *
    */
    public int getNumDoors() {
        return doors.size();
    }

    /**
    * Adds Door object to HashMap of doors in room.
    * @param dir The direction of the door
    * @param newDoor The Door to be added
    *
    */
    public void addDoor(String dir, Door newDoor) {
        doors.put(dir, newDoor);
    }


    /**
    * Returns whether the player is in the room as a boolean.
    * @return Whether the player is in the room
    *
    */
    public boolean isPlayerInRoom() {
        return inRoom;
    }


    /**
    * Sets whether the player is in the room as a boolean.
    * @param newInRoom Boolean representing whether the player is in the room
    *
    */
    public void setIsPlayerInRoom(boolean newInRoom) {
        inRoom = newInRoom;
    }

    /**
    * Replaces character in String str at location element with letter.
    * @param str The string being modified
    * @param element The element in the string to be replaced
    * @param letter The character to be inserted in the string
    * @return The updated string
    */
    protected String replaceCharInString(String str, int element, char letter) {
        int length = str.length();
        if (element == 0) {
            return (letter + str.substring(element + 1));
        } else if (element == length - 1) {
            return (str.substring(0, length - 1) + letter);
        } else {
            return (str.substring(0, element) + letter + str.substring(element + 1));
        }
    }


    /**
    * Creates and returns a string to represent the top and bottom walls of the room.
    * @return The string for top and bottom rows
    */
    protected String drawTopBottomRow() {
        int i;
        String str = new String(nsWallSymbol.toString());

        for (i = 0; i < width - 1; i++) {
            str += nsWallSymbol.toString();
        }
        return str;
    }


    /**
    * Creates and returns a string to represent the middle rows of the room.
    * @return The string for middle rows
    */
    protected String drawMiddleRow() {
        int i;
        String str = new String(floorSymbol.toString());

        for (i = 0; i < width - 1; i++) {
            str += floorSymbol.toString();
        }
        str = replaceCharInString(str, 0, ewWallSymbol);
        str = replaceCharInString(str, width - 1, ewWallSymbol);
        return str;
    }


    /**
    * Creates and returns an ArrayList of Strings to represent the entire room.
    * @param middle The string representing the middle rows of the room
    * @param topBottom The string representing the top and bottom rows of the room
    * @return The ArrayList of Strings representing the room
    */
    protected ArrayList<String> drawRows(String middle, String topBottom) {
        ArrayList<String> roomStringArray = new ArrayList<String>();
        int i;

        for (i = 0; i < height; i++) {
            roomStringArray.add(middle);
        }
        roomStringArray.set(0, topBottom);
        roomStringArray.set(height - 1, topBottom);

        return roomStringArray;
    }

    /**
    * Inserts door symbols in strings representing the room where appropriate.
    * @param roomStringArray The ArrayList of strings representing the room
    */
    protected void drawDoors(ArrayList<String> roomStringArray) {
        if (getNDoor() != null) {
            roomStringArray.set(0, replaceCharInString(roomStringArray.get(0), getNDoor().getPosition(), doorSymbol));
        }
        if (getEDoor() != null) {
            roomStringArray.set(getEDoor().getPosition(),
                                replaceCharInString(roomStringArray.get(getEDoor().getPosition()),
                                getWidth() - 1, doorSymbol));
        }
        if (getSDoor() != null) {
            roomStringArray.set(getHeight() - 1, replaceCharInString(roomStringArray.get(getHeight() - 1),
                                getSDoor().getPosition(), doorSymbol));
        }
        if (getWDoor() != null) {
            roomStringArray.set(getWDoor().getPosition(),
                                replaceCharInString(roomStringArray.get(getWDoor().getPosition()),
                                0, doorSymbol));
        }
    }


    protected Door getNDoor() {
        return doors.get("N");
    }


    protected Door getEDoor() {
        return doors.get("E");
    }


    protected Door getSDoor() {
        return doors.get("S");
    }


    protected Door getWDoor() {
        return doors.get("W");
    }


    /**
    * Inserts item symbols in strings representing the room where appropriate.
    * @param roomStringArray The ArrayList of strings representing the room
    */
    protected void drawItems(ArrayList<String> roomStringArray) {
        int x;
        int y;

        for (Item i : items) {
            x = (int) i.getXyLocation().getX();
            y = (int) i.getXyLocation().getY();

            roomStringArray.set(y, replaceCharInString(roomStringArray.get(y), x, i.getDisplayCharacter()));
        }
    }


    /**
    * Inserts player symbol in strings representing the room where appropriate.
    * @param roomStringArray The ArrayList of strings representing the room
    */
    protected void drawPlayer(ArrayList<String> roomStringArray) {
        if (inRoom) {
            int x;
            int y;

            x = (int) player.getXyLocation().getX();
            y = (int) player.getXyLocation().getY();

            roomStringArray.set(y, replaceCharInString(roomStringArray.get(y), x, playerSymbol));
        }
    }


    /**
    * Assembles and returns ArrayList of strings representing the room by using helper methods.
    * @return The ArrayList of strings representing the room
    */
    protected ArrayList<String> assembleRoom() {
        String topBottom = drawTopBottomRow();
        String middle = drawMiddleRow();
        int i;
        ArrayList<String> roomStringArray = drawRows(middle, topBottom);

        drawDoors(roomStringArray);
        drawItems(roomStringArray);
        drawPlayer(roomStringArray);

        return roomStringArray;
    }


    /**
    * Assembles an ArrayList of strings representing the room, then converts them into a single
    * String, with rows of the room separated by newlines.
    * @return The final String representing the room
    */
    public String displayRoom() {
        ArrayList<String> roomStringArray = assembleRoom();
        int i;

        roomDescription = "";

        for (i = 0; i < roomStringArray.size(); i++) {
            roomDescription += roomStringArray.get(i) + "\n";
        }

        return roomDescription;
    }


    /**
    * Sets the Rogue that the Room belongs to.
    * @param newRogue The Rogue that the Room belongs to
    *
    */
    public void setRogue(Rogue newRogue) {
        rogue = newRogue;
    }


    /**
    * Tests if a newly added item is in the walls.
    * @param x The Point location for the new item
    * @param testRoom The room that the point is in
    * @throws ImpossiblePositionException if the item would be in a wall
    *
    */
    protected void pointWallTest(Point x, Room testRoom) throws ImpossiblePositionException {
        if ((x.getX() == 0) || (x.getX() == testRoom.getWidth() - 1) || (x.getY() == 0)
            || (x.getY() == testRoom.getHeight() - 1)) {
            throw new ImpossiblePositionException();
        }
    }


    /**
    * Tests if a newly added item overlaps an existing item.
    * @param toAdd The new item being tested
    * @param testRoom The room that the new item is in
    * @throws ImpossiblePositionException if the item would overlap an existing item
    *
    */
    protected void pointItemTest(Item toAdd, Room testRoom) throws ImpossiblePositionException {
        Point existingPos = new Point();
        Point itemPos = toAdd.getXyLocation();
        for (Item nextItem:items) {
            existingPos = nextItem.getXyLocation();
            if ((itemPos.getX() == existingPos.getX()) && (itemPos.getY() == existingPos.getY())
                && (toAdd.getId() != nextItem.getId())) {
                throw new ImpossiblePositionException();
            }
        }

    }


    /**
    * Tests if a newly added item overlaps the player.
    * @param x The Point location for the new item
    * @param testRoom The room that the player is in
    * @throws ImpossiblePositionException if the item would overlap the player
    *
    */
    protected void pointPlayerTest(Point x, Room testRoom) throws ImpossiblePositionException {
        Point existingPos = player.getXyLocation();
        if ((x.getX() == existingPos.getX()) && (x.getY() == existingPos.getY())) {
            throw new ImpossiblePositionException();
        }

    }

    /**
    * Verifies that the item to add exists in Rogue.
    * @param toAdd The item that is being tested
    * @throws ImpossiblePositionException if the item does not exist in Rogue
    *
    */
    protected void addItemInRogueTest(Item toAdd) throws NoSuchItemException {
        boolean itemExists = false;
        for (Item rogueItem:rogue.getItems()) {
            if (toAdd.getId() == rogueItem.getId()) {
                itemExists = true;
                break;
            }
        }
        if (!itemExists) {
            throw new NoSuchItemException();
        }
    }


    /**
    * Tests whether an item to add to the room overlaps walls, other items, or the player. Also verifies
    * that the item exists in rogue. If there are no conflicts the item is added to the room.
    * @param toAdd The item being added to the room
    * @throws ImpossiblePositionException if the item overlaps anything else
    * @throws NoSuchItemException if the items does not exist in Rogue
    *
    */
    public void addItem(Item toAdd) throws ImpossiblePositionException, NoSuchItemException {
        Point x = toAdd.getXyLocation();
        try {
            pointWallTest(x, this);
            pointItemTest(toAdd, this);
            pointPlayerTest(x, this);
            addItemInRogueTest(toAdd);
            items.add(toAdd);
        } catch (ImpossiblePositionException e) {
            throw e;
        } catch (NoSuchItemException e) {
            int itemId = toAdd.getId();
            for (int i = 0; i < items.size(); i++) {
                if (itemId == items.get(i).getId()) {
                    removeItem(i);
                    i--;
                }
            }
            throw e;
        }
    }


    protected void removeItem(int i) {
        items.remove(i);
    }


    protected void removeItemByPos(Point pos) {
        int i = 0;
        for (Item theItem:items) {
            if ((pos.getX() == theItem.getXyLocation().getX()) && (pos.getX() == theItem.getXyLocation().getX())) {
                //items.remove(i);
                i++;
            }
            i++;
        }
    }


    protected void removeItemByItem(Item toRemove) {
        items.remove(toRemove);
    }

    /**
    * Tests that input room has at least one door, has everything in valid places, and that doors all have connections.
    * @param testRoom The room to be verified
    * @return Whether the room is valid, as boolean
    * @throws NotEnoughDoorsException if the room does not have at least one door
    *
    */
    public boolean verifyRoom(Room testRoom) throws NotEnoughDoorsException {
        for (Item i:testRoom.getRoomItems()) {
            try {
                pointWallTest(i.getXyLocation(), testRoom);
                pointItemTest(i, testRoom);
                pointPlayerTest(i.getXyLocation(), testRoom);
            } catch (ImpossiblePositionException e) {
                return false;
            }
        }

        if ((testRoom.getNDoor() == null) && (testRoom.getEDoor() == null) && (testRoom.getSDoor() == null)
            && (testRoom.getWDoor() == null)) {
            throw new NotEnoughDoorsException();
            // After catching, add a door to the room and try to connect it to any other
            // room that has less than four doors
        }
        return true;
    }


    /**
    * Tests that this room has at least one door, has everything in valid places, and that doors all have connections.
    * @return Whether the room is valid, as boolean
    * @throws NotEnoughDoorsException if the room does not have at least one door
    *
    */
    public boolean verifyRoom() throws NotEnoughDoorsException {
        for (Item i:this.getRoomItems()) {
            try {
                pointWallTest(i.getXyLocation(), this);
                pointItemTest(i, this);
                pointPlayerTest(i.getXyLocation(), this);
            } catch (ImpossiblePositionException e) {
                return false;
            }
        }

        if ((this.getNDoor() == null) && (this.getEDoor() == null) && (this.getSDoor() == null)
            && (this.getWDoor() == null)) {
            throw new NotEnoughDoorsException();
            // After catching, add a door to the room and try to connect it to any other
            // room that has less than four doors
        }
        return true;
    }


    /**
    * Adds a door to the room
    * @param dir The direction of the door
    * @param pos The position of the door
    *
    */
    public void setDoor(String dir, int pos) {
        doors.put(dir, new Door(dir.charAt(0), pos));
    }
}
