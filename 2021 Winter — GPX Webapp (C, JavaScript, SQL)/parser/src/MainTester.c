#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "GPXParser.h"
#include "LinkedListAPI.h"


/*
List* makeGpxDataList(){
    List *newList = malloc(sizeof(List));

    newList->head = NULL;
    newList->tail = NULL;
    newList->length = 0;
    newList->deleteData = deleteGpxData;
    newList->compare = compareGpxData;
    newList->printData = gpxDataToString;

    return newList;
}


List* makeTrkSegList(){
    List *newList = malloc(sizeof(List));

    newList->head = NULL;
    newList->tail = NULL;
    newList->length = 0;
    newList->deleteData = deleteTrackSegment;
    newList->compare = compareTrackSegments;
    newList->printData = trackSegmentToString;

    return newList;
}
*/

void printWaypoints(GPXdoc *doc) {
    int i = 0;
    Node* cur_node = NULL;

    cur_node = doc->waypoints->head;
    printf("%d waypoints\n", doc->waypoints->length);
    for (i = 0; i < doc->waypoints->length; i++) {
        printf("Waypoint %d\n", i + 1);
        printf("Name: %s\n", ((Waypoint*)(cur_node->data))->name);
        printf("Longitude: %f\n", ((Waypoint*)(cur_node->data))->longitude);
        printf("Latitude: %f\n", ((Waypoint*)(cur_node->data))->latitude);
        cur_node = cur_node->next;
    }
}


void testListFunctions(GPXdoc *doc) {
    List *testList;

    printf("\n\n\nStarting list function test\n\n");

    doc->waypoints->deleteData(NULL);
    doc->waypoints->compare(NULL, NULL);
    doc->waypoints->printData(NULL);

    doc->routes->deleteData(NULL);
    doc->routes->compare(NULL, NULL);
    doc->routes->printData(NULL);

    doc->tracks->deleteData(NULL);
    doc->tracks->compare(NULL, NULL);
    doc->tracks->printData(NULL);

    /*
    testList = makeGpxDataList();
    testList->deleteData(NULL);
    testList->compare(NULL, NULL);
    testList->printData(NULL);

    testList = makeTrkSegList();
    testList->deleteData(NULL);
    testList->compare(NULL, NULL);
    testList->printData(NULL);
    */

    printf("\nFinished list function test\n\n");

    return;
}



void testCreateGPXdoc(char* fileName) {
    char *str = NULL;
    Waypoint *wpt = NULL;
    Route *rte = NULL;
    Track *trk = NULL;
    Node *theNode = NULL;
    printf("Testing createGPXdoc()\n");
    /*char result = 'a';*/
    GPXdoc *result = createGPXdoc(fileName);

    printf("GPXdoc pointer retrieved\n");

    printf("Result:  %p\n", result);

    str = GPXdocToString(result);
    //printf("String version:\n%s\n", str);
    if (str != NULL) {
        free(str);
    }

    //testListFunctions(result);
    //printWaypoints(result);

    printf("Waypoints: %d\n", getNumWaypoints(result));
    printf("Routes: %d\n", getNumRoutes(result));
    printf("Tracks: %d\n", getNumTracks(result));
    printf("Track segments: %d\n", getNumSegments(result));
    printf("GPX data: %d\n", getNumGPXData(result));

    wpt = getWaypoint(result, "Steele Cabin");
    if (wpt == NULL) {
        printf("wpt = NULL\n");
    }
    /*printf("Found waypoint: %s\n", wpt->name);
    printf("Latitude: %f\n", wpt->latitude);
    printf("Longitude: %f\n", wpt->longitude);*/

    rte = getRoute(result, "Some route");
    if (rte == NULL) {
        printf("rte = NULL\n");
    }
    //printf("Found route: %s\n", rte->name);

    trk = getTrack(result, "Mount Steele Trail");
    if (trk == NULL) {
        printf("trk = NULL\n");
    }
    //printf("Found waypoint: %s\n", trk->name);

    /*printf("Calling gpxDocToXMLDoc.\n");
    writeGPXdoc(result, "output2.xml");

    if (result->routes->head != NULL) {
        printf("Distance: %f\n", getRouteLen(result->routes->head->data));
    }

    if (result->tracks->head != NULL) {
        theNode = result->tracks->head;
        while (theNode != NULL) {
            printf("Track distance: %f\n", getTrackLen(theNode->data));
            theNode = theNode->next;
        }
        
    }*/

    /*
    str = routeToJSON(result->routes->head->data);
    printf("%s\n", str);
    free(str);
    str = routeListToJSON(result->routes);
    printf("%s\n", str);
    free(str);
    */
    
    /*
    str = trackToJSON(result->tracks->head->data);
    printf("%s\n", str);
    free (str);
    str = trackListToJSON(result->tracks);
    printf("%s\n", str);
    free (str);
    */

    //addWaypoint(result->routes->head->data, result->waypoints->head->data);

    /*
    str = GPXtoJSON(result);
    printf("%s\n", str);
    free(str);
    */

    deleteGPXdoc(result);


    char inputstring[200];

    strcpy(inputstring, "{\"version\":03.14,\"creator\":\"creatorValue\"}\0");


    printf("Running JSONtoGPX\n");
    result = JSONtoGPX(inputstring);
    free(str);
    str = GPXdocToString(result);
    printf("%s\n", str);
    free(str);
    str = GPXtoJSON(result);
    printf("%s\n", str);
    free(str);

    strcpy(inputstring, "{\"lat\":43.537299,\"lon\":-80.218267}\0");
    wpt = JSONtoWaypoint(inputstring);
    str = waypointToString(wpt);
    printf("%s\n", str);
    free(str);

    strcpy(inputstring, "\"name\":\"Reynolds walk\"\0");
    rte = JSONtoRoute(inputstring);
    str = routeToString(rte);
    printf("%s\n", str);
    free(str);


    deleteGPXdoc(result);
}


int main(int argc, char **argv) {
    printf("Program running\n");

    if (argc != 2){
        printf("usage: xmlExample <someXMLfile>\n");

        return(1);
    }

    testCreateGPXdoc(argv[1]);
    xmlCleanupParser();

    return 0;
}