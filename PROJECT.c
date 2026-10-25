#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LOCATIONS 50
#define NAME_LEN 50
#define INF 1000000000

/* =========================================================
   DATA STRUCTURES
   ========================================================= */

typedef struct Edge {
    int destination;
    int distance;       // Distance in km
    int time;           // Normal travel time in minutes
    int traffic;        // 1 = Low, 2 = Medium, 3 = High
    int blocked;        // 0 = Open, 1 = Blocked

    struct Edge *next;
} Edge;


typedef struct {
    char name[NAME_LEN];

    int isHospital;
    int isPetrolPump;
    int isRailwayStation;
    int isAirport;
    int isPoliceStation;

    Edge *head;
} Location;


typedef struct {
    int node;
    int distance;
} HeapNode;


typedef struct {
    HeapNode arr[MAX_LOCATIONS * 10];
    int size;
} MinHeap;


/* =========================================================
   GLOBAL VARIABLES
   ========================================================= */

Location city[MAX_LOCATIONS];
int locationCount = 0;


/* =========================================================
   INPUT UTILITY
   ========================================================= */

void clearInputBuffer()
{
    int c;

    while ((c = getchar()) != '\n' && c != EOF);
}


/* =========================================================
   CASE-INSENSITIVE STRING COMPARISON
   Works without strcasecmp()
   ========================================================= */

int stringsEqualIgnoreCase(const char *a, const char *b)
{
    while (*a && *b)
    {
        char ca = *a;
        char cb = *b;

        if (ca >= 'A' && ca <= 'Z')
            ca = ca + ('a' - 'A');

        if (cb >= 'A' && cb <= 'Z')
            cb = cb + ('a' - 'A');

        if (ca != cb)
            return 0;

        a++;
        b++;
    }

    return (*a == '\0' && *b == '\0');
}


/* =========================================================
   FIND LOCATION
   ========================================================= */

int findLocation(const char *name)
{
    for (int i = 0; i < locationCount; i++)
    {
        if (stringsEqualIgnoreCase(city[i].name, name))
            return i;
    }

    return -1;
}


/* =========================================================
   TRAFFIC FUNCTIONS
   ========================================================= */

const char *trafficName(int traffic)
{
    if (traffic == 1)
        return "LOW";

    if (traffic == 2)
        return "MEDIUM";

    return "HIGH";
}


int trafficMultiplier(int traffic)
{
    if (traffic == 1)
        return 1;

    if (traffic == 2)
        return 2;

    return 3;
}


/* =========================================================
   MIN HEAP
   ========================================================= */

void swapHeapNode(HeapNode *a, HeapNode *b)
{
    HeapNode temp = *a;

    *a = *b;
    *b = temp;
}


void heapPush(MinHeap *heap, int node, int distance)
{
    int i = heap->size;

    heap->arr[i].node = node;
    heap->arr[i].distance = distance;

    heap->size++;

    while (i > 0)
    {
        int parent = (i - 1) / 2;

        if (heap->arr[parent].distance <=
            heap->arr[i].distance)
        {
            break;
        }

        swapHeapNode(
&heap->arr[parent],
&heap->arr[i]
        );

        i = parent;
    }
}


HeapNode heapPop(MinHeap *heap)
{
    HeapNode result = heap->arr[0];

    heap->size--;

    if (heap->size > 0)
    {
        heap->arr[0] =
            heap->arr[heap->size];

        int i = 0;

        while (1)
        {
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int smallest = i;

            if (left < heap->size &&
                heap->arr[left].distance <
                heap->arr[smallest].distance)
            {
                smallest = left;
            }

            if (right < heap->size &&
                heap->arr[right].distance <
                heap->arr[smallest].distance)
            {
                smallest = right;
            }

            if (smallest == i)
                break;

            swapHeapNode(
&heap->arr[i],
&heap->arr[smallest]
            );

            i = smallest;
        }
    }

    return result;
}


/* =========================================================
   ADD LOCATION
   ========================================================= */

void addLocation()
{
    if (locationCount >= MAX_LOCATIONS)
    {
        printf("\nMaximum number of locations reached.\n");
        return;
    }

    char name[NAME_LEN];

    printf("\nEnter location name: ");

    fgets(name, NAME_LEN, stdin);

    name[strcspn(name, "\n")] = '\0';

    if (findLocation(name) != -1)
    {
        printf("Location already exists.\n");
        return;
    }

    strcpy(city[locationCount].name, name);

    city[locationCount].isHospital = 0;
    city[locationCount].isPetrolPump = 0;
    city[locationCount].isRailwayStation = 0;
    city[locationCount].isAirport = 0;
    city[locationCount].isPoliceStation = 0;

    city[locationCount].head = NULL;

    printf("\nSelect location type:\n");

    printf("1. Normal\n");
    printf("2. Hospital\n");
    printf("3. Petrol Pump\n");
    printf("4. Railway Station\n");
    printf("5. Airport\n");
    printf("6. Police Station\n");

    int type;

    printf("Choice: ");
    scanf("%d", &type);

    clearInputBuffer();

    if (type == 2)
        city[locationCount].isHospital = 1;

    else if (type == 3)
        city[locationCount].isPetrolPump = 1;

    else if (type == 4)
        city[locationCount].isRailwayStation = 1;

    else if (type == 5)
        city[locationCount].isAirport = 1;

    else if (type == 6)
        city[locationCount].isPoliceStation = 1;

    locationCount++;

    printf("\nLocation added successfully!\n");
}


/* =========================================================
   ADD ROAD
   ========================================================= */

void addRoad()
{
    char source[NAME_LEN];
    char destination[NAME_LEN];

    printf("\nEnter source location: ");

    fgets(source, NAME_LEN, stdin);
    source[strcspn(source, "\n")] = '\0';

    printf("Enter destination location: ");

    fgets(destination, NAME_LEN, stdin);
    destination[strcspn(destination, "\n")] = '\0';

    int u = findLocation(source);
    int v = findLocation(destination);

    if (u == -1 || v == -1)
    {
        printf("\nOne or both locations do not exist.\n");
        return;
    }

    int distance;
    int time;

    printf("\nDistance between locations (km): ");
    scanf("%d", &distance);

    printf("Normal travel time (minutes): ");
    scanf("%d", &time);

    clearInputBuffer();

    Edge *e1 =
        (Edge *)malloc(sizeof(Edge));

    Edge *e2 =
        (Edge *)malloc(sizeof(Edge));

    if (e1 == NULL || e2 == NULL)
    {
        printf("Memory allocation failed.\n");

        free(e1);
        free(e2);

        return;
    }

    /* u -> v */

    e1->destination = v;
    e1->distance = distance;
    e1->time = time;
    e1->traffic = 1;
    e1->blocked = 0;

    e1->next = city[u].head;

    city[u].head = e1;


    /* v -> u */

    e2->destination = u;
    e2->distance = distance;
    e2->time = time;
    e2->traffic = 1;
    e2->blocked = 0;

    e2->next = city[v].head;

    city[v].head = e2;

    printf("\nRoad added successfully!\n");
}


/* =========================================================
   FIND EDGE
   ========================================================= */

Edge *findEdge(int u, int v)
{
    Edge *e = city[u].head;

    while (e != NULL)
    {
        if (e->destination == v)
            return e;

        e = e->next;
    }

    return NULL;
}


/* =========================================================
   DISPLAY LOCATIONS
   ========================================================= */

void displayLocations()
{
    printf("\n========================================\n");
    printf("              LOCATIONS\n");
    printf("========================================\n");

    for (int i = 0; i < locationCount; i++)
    {
        printf("%d. %s",
               i + 1,
               city[i].name);

        if (city[i].isHospital)
            printf(" [HOSPITAL]");

        if (city[i].isPetrolPump)
            printf(" [PETROL PUMP]");

        if (city[i].isRailwayStation)
            printf(" [RAILWAY]");

        if (city[i].isAirport)
            printf(" [AIRPORT]");

        if (city[i].isPoliceStation)
            printf(" [POLICE]");

        printf("\n");
    }
}


/* =========================================================
   DISPLAY ROADS
   ========================================================= */

void displayRoads()
{
    printf("\n========================================\n");
    printf("                ROADS\n");
    printf("========================================\n");

    for (int i = 0; i < locationCount; i++)
    {
        Edge *e = city[i].head;

        while (e != NULL)
        {
            /*
               Since roads are bidirectional,
               print each road only once.
            */

            if (i < e->destination)
            {
                printf("\n%s <-> %s\n",
                       city[i].name,
                       city[e->destination].name);

                printf("Distance : %d km\n",
                       e->distance);

                printf("Time     : %d minutes\n",
                       e->time);

                printf("Traffic  : %s\n",
                       trafficName(e->traffic));

                printf("Status   : %s\n",
                       e->blocked ?
                       "BLOCKED" :
                       "OPEN");
            }

            e = e->next;
        }
    }
}


/* =========================================================
   CALCULATE EDGE WEIGHT
   ========================================================= */

int calculateWeight(Edge *e, int mode)
{
    if (e->blocked)
        return INF;

    /*
       MODE 1 = SHORTEST DISTANCE
       MODE 2 = FASTEST ROUTE
       MODE 3 = LOW TRAFFIC ROUTE
    */

    if (mode == 1)
    {
        return e->distance;
    }

    if (mode == 2)
    {
        /*
           Traffic increases travel time.
        */

        return e->time *
               trafficMultiplier(e->traffic);
    }

    if (mode == 3)
    {
        /*
           Low traffic route.
        */

        return e->time *
               trafficMultiplier(e->traffic);
    }

    return e->distance;
}


/* =========================================================
   DIJKSTRA ALGORITHM
   ========================================================= */

void dijkstra(
    int source,
    int mode,
    int dist[],
    int parent[]
)
{
    for (int i = 0; i < locationCount; i++)
    {
        dist[i] = INF;
        parent[i] = -1;
    }

    MinHeap heap;

    heap.size = 0;

    dist[source] = 0;

    heapPush(
&heap,
        source,
        0
    );

    while (heap.size > 0)
    {
        HeapNode current =
            heapPop(&heap);

        int u = current.node;

        /*
           Ignore outdated heap entries.
        */

        if (current.distance != dist[u])
            continue;

        Edge *e = city[u].head;

        while (e != NULL)
        {
            int v = e->destination;

            int weight =
                calculateWeight(e, mode);

            if (weight != INF)
            {
                if (dist[u] + weight < dist[v])
                {
                    dist[v] =
                        dist[u] + weight;

                    parent[v] = u;

                    heapPush(
&heap,
                        v,
                        dist[v]
                    );
                }
            }

            e = e->next;
        }
    }
}


/* =========================================================
   PRINT ROUTE
   ========================================================= */

void printRoute(
    int parent[],
    int source,
    int destination
)
{
    int path[MAX_LOCATIONS];

    int count = 0;

    int current = destination;

    while (current != -1)
    {
        path[count++] = current;

        if (current == source)
            break;

        current = parent[current];
    }

    if (path[count - 1] != source)
    {
        printf("\nNo route available.\n");
        return;
    }

    printf("\nRoute:\n\n");

    for (int i = count - 1; i >= 0; i--)
    {
        printf("  %s",
               city[path[i]].name);

        if (i > 0)
        {
            printf("\n    |\n");
            printf("    v\n");
        }
    }

    printf("\n");
}


/* =========================================================
   FIND ROUTE
   ========================================================= */

void findRoute()
{
    char sourceName[NAME_LEN];
    char destinationName[NAME_LEN];

    printf("\nEnter source: ");

    fgets(sourceName, NAME_LEN, stdin);
    sourceName[strcspn(sourceName, "\n")] = '\0';

    printf("Enter destination: ");

    fgets(destinationName, NAME_LEN, stdin);
    destinationName[strcspn(destinationName, "\n")] = '\0';

    int source =
        findLocation(sourceName);

    int destination =
        findLocation(destinationName);

    if (source == -1 ||
        destination == -1)
    {
        printf("\nLocation not found.\n");
        return;
    }

    printf("\n========================================\n");
    printf("              ROUTE TYPE\n");
    printf("========================================\n");

    printf("1. Shortest Distance\n");
    printf("2. Fastest Route\n");
    printf("3. Low Traffic Route\n");

    int mode;

    printf("\nChoice: ");
    scanf("%d", &mode);

    clearInputBuffer();

    if (mode < 1 || mode > 3)
    {
        printf("Invalid choice.\n");
        return;
    }

    int dist[MAX_LOCATIONS];
    int parent[MAX_LOCATIONS];

    dijkstra(
        source,
        mode,
        dist,
        parent
    );

    if (dist[destination] == INF)
    {
        printf("\nNo route is currently available.\n");
        return;
    }

    printf("\n========================================\n");
    printf("              BEST ROUTE\n");
    printf("========================================\n");

    printRoute(
        parent,
        source,
        destination
    );

    if (mode == 1)
    {
        printf("Total distance: %d km\n",
               dist[destination]);
    }
    else
    {
        printf("Estimated travel time: %d minutes\n",
               dist[destination]);
    }
}


/* =========================================================
   UPDATE TRAFFIC
   ========================================================= */

void updateTraffic()
{
    char source[NAME_LEN];
    char destination[NAME_LEN];

    printf("\nEnter first location: ");

    fgets(source, NAME_LEN, stdin);
    source[strcspn(source, "\n")] = '\0';

    printf("Enter second location: ");

    fgets(destination, NAME_LEN, stdin);
    destination[strcspn(destination, "\n")] = '\0';

    int u = findLocation(source);
    int v = findLocation(destination);

    if (u == -1 || v == -1)
    {
        printf("\nLocation not found.\n");
        return;
    }

    Edge *e1 =
        findEdge(u, v);

    Edge *e2 =
        findEdge(v, u);

    if (e1 == NULL)
    {
        printf("\nRoad does not exist.\n");
        return;
    }

    printf("\nCurrent traffic: %s\n",
           trafficName(e1->traffic));

    printf("\n1. LOW\n");
    printf("2. MEDIUM\n");
    printf("3. HIGH\n");

    int level;

    printf("\nNew traffic level: ");
    scanf("%d", &level);

    clearInputBuffer();

    if (level < 1 || level > 3)
    {
        printf("Invalid traffic level.\n");
        return;
    }

    e1->traffic = level;
    e2->traffic = level;

    printf("\nTraffic updated successfully!\n");
}


/* =========================================================
   BLOCK / UNBLOCK ROAD
   ========================================================= */

void toggleRoad()
{
    char source[NAME_LEN];
    char destination[NAME_LEN];

    printf("\nEnter first location: ");

    fgets(source, NAME_LEN, stdin);
    source[strcspn(source, "\n")] = '\0';

    printf("Enter second location: ");

    fgets(destination, NAME_LEN, stdin);
    destination[strcspn(destination, "\n")] = '\0';

    int u = findLocation(source);
    int v = findLocation(destination);

    if (u == -1 || v == -1)
    {
        printf("\nLocation not found.\n");
        return;
    }

    Edge *e1 =
        findEdge(u, v);

    Edge *e2 =
        findEdge(v, u);

    if (e1 == NULL)
    {
        printf("\nRoad does not exist.\n");
        return;
    }

    e1->blocked =
        !e1->blocked;

    e2->blocked =
        e1->blocked;

    if (e1->blocked)
    {
        printf("\nRoad BLOCKED successfully!\n");
    }
    else
    {
        printf("\nRoad UNBLOCKED successfully!\n");
    }
}


/* =========================================================
   NEAREST FACILITY
   ========================================================= */

void nearestFacility()
{
    char sourceName[NAME_LEN];

    printf("\nEnter your current location: ");

    fgets(sourceName, NAME_LEN, stdin);
    sourceName[strcspn(sourceName, "\n")] = '\0';

    int source =
        findLocation(sourceName);

    if (source == -1)
    {
        printf("\nLocation not found.\n");
        return;
    }

    printf("\n========================================\n");
    printf("          FIND NEAREST FACILITY\n");
    printf("========================================\n");

    printf("1. Hospital\n");
    printf("2. Petrol Pump\n");
    printf("3. Railway Station\n");
    printf("4. Airport\n");
    printf("5. Police Station\n");

    int choice;

    printf("\nChoice: ");
    scanf("%d", &choice);

    clearInputBuffer();

    if (choice < 1 || choice > 5)
    {
        printf("Invalid choice.\n");
        return;
    }

    int dist[MAX_LOCATIONS];
    int parent[MAX_LOCATIONS];

    dijkstra(
        source,
        1,
        dist,
        parent
    );

    int nearest = -1;
    int minimum = INF;

    for (int i = 0; i < locationCount; i++)
    {
        int valid = 0;

        if (choice == 1 &&
            city[i].isHospital)
            valid = 1;

        else if (choice == 2 &&
                 city[i].isPetrolPump)
            valid = 1;

        else if (choice == 3 &&
                 city[i].isRailwayStation)
            valid = 1;

        else if (choice == 4 &&
                 city[i].isAirport)
            valid = 1;

        else if (choice == 5 &&
                 city[i].isPoliceStation)
            valid = 1;

        if (valid &&
            i != source &&
            dist[i] < minimum)
        {
            minimum = dist[i];
            nearest = i;
        }
    }

    if (nearest == -1 ||
        minimum == INF)
    {
        printf("\nNo reachable facility found.\n");
        return;
    }

    printf("\nNearest facility: %s\n",
           city[nearest].name);

    printf("Distance: %d km\n",
           minimum);

    printRoute(
        parent,
        source,
        nearest
    );
}


/* =========================================================
   EMERGENCY MODE
   ========================================================= */

void emergencyMode()
{
    char ambulance[NAME_LEN];

    printf("\n========================================\n");
    printf("           EMERGENCY MODE\n");
    printf("========================================\n");

    printf("\nEnter ambulance/current location: ");

    fgets(
        ambulance,
        NAME_LEN,
        stdin
    );

    ambulance[
        strcspn(ambulance, "\n")
    ] = '\0';

    int source =
        findLocation(ambulance);

    if (source == -1)
    {
        printf("\nLocation not found.\n");
        return;
    }

    int dist[MAX_LOCATIONS];
    int parent[MAX_LOCATIONS];

    /*
       Mode 2 = Fastest route
    */

    dijkstra(
        source,
        2,
        dist,
        parent
    );

    int hospital = -1;
    int best = INF;

    for (int i = 0; i < locationCount; i++)
    {
        if (city[i].isHospital &&
            dist[i] < best)
        {
            best = dist[i];
            hospital = i;
        }
    }

    if (hospital == -1 ||
        best == INF)
    {
        printf("\nNo reachable hospital found.\n");
        return;
    }

    printf("\n========================================\n");
    printf("           EMERGENCY ROUTE\n");
    printf("========================================\n");

    printf("\nDestination Hospital: %s\n",
           city[hospital].name);

    printf("Estimated Time: %d minutes\n",
           best);

    printRoute(
        parent,
        source,
        hospital
    );

    printf("\nBlocked roads were automatically avoided.\n");
}


/* =========================================================
   SEARCH LOCATION
   ========================================================= */

void searchLocation()
{
    char name[NAME_LEN];

    printf("\nEnter location to search: ");

    fgets(name, NAME_LEN, stdin);

    name[strcspn(name, "\n")] = '\0';

    int index =
        findLocation(name);

    if (index == -1)
    {
        printf("\nLocation not found.\n");
        return;
    }

    printf("\n========================================\n");
    printf("           LOCATION DETAILS\n");
    printf("========================================\n");

    printf("Name: %s\n",
           city[index].name);

    printf("\nConnected roads:\n");

    Edge *e =
        city[index].head;

    while (e != NULL)
    {
        printf("\n-> %s\n",
               city[e->destination].name);

        printf("   Distance: %d km\n",
               e->distance);

        printf("   Time: %d minutes\n",
               e->time);

        printf("   Traffic: %s\n",
               trafficName(e->traffic));

        printf("   Status: %s\n",
               e->blocked ?
               "BLOCKED" :
               "OPEN");

        e = e->next;
    }
}


/* =========================================================
   ROUTE COMPARISON
   ========================================================= */

void compareRoutes()
{
    char sourceName[NAME_LEN];
    char destinationName[NAME_LEN];

    printf("\nSource: ");

    fgets(
        sourceName,
        NAME_LEN,
        stdin
    );

    sourceName[
        strcspn(sourceName, "\n")
    ] = '\0';


    printf("Destination: ");

    fgets(
        destinationName,
        NAME_LEN,
        stdin
    );

    destinationName[
        strcspn(destinationName, "\n")
    ] = '\0';


    int source =
        findLocation(sourceName);

    int destination =
        findLocation(destinationName);


    if (source == -1 ||
        destination == -1)
    {
        printf("\nLocation not found.\n");
        return;
    }


    int dist[MAX_LOCATIONS];
    int parent[MAX_LOCATIONS];


    printf("\n========================================\n");
    printf("            ROUTE COMPARISON\n");
    printf("========================================\n");


    /* Shortest Distance */

    dijkstra(
        source,
        1,
        dist,
        parent
    );

    if (dist[destination] == INF)
    {
        printf("\nShortest Distance: No route");
    }
    else
    {
        printf("\nShortest Distance: %d km",
               dist[destination]);
    }


    /* Fastest Route */

    dijkstra(
        source,
        2,
        dist,
        parent
    );

    if (dist[destination] == INF)
    {
        printf("\nFastest Route: No route");
    }
    else
    {
        printf("\nFastest Route: %d minutes",
               dist[destination]);
    }


    /* Low Traffic Route */

    dijkstra(
        source,
        3,
        dist,
        parent
    );

    if (dist[destination] == INF)
    {
        printf("\nLow Traffic Route: No route\n");
    }
    else
    {
        printf("\nLow Traffic Route: %d minutes\n",
               dist[destination]);
    }
}


/* =========================================================
   CREATE SAMPLE CITY
   ========================================================= */

void createSampleCity()
{
    const char *names[] =
    {
        "University",
        "Railway Station",
        "Airport",
        "City Mall",
        "Central Hospital",
        "Police Station",
        "Bus Stand",
        "Market",
        "Tech Park",
        "Hotel"
    };


    int totalLocations =
        sizeof(names) / sizeof(names[0]);


    for (int i = 0;
         i < totalLocations;
         i++)
    {
        strcpy(
            city[locationCount].name,
            names[i]
        );

        city[locationCount].isHospital = 0;
        city[locationCount].isPetrolPump = 0;
        city[locationCount].isRailwayStation = 0;
        city[locationCount].isAirport = 0;
        city[locationCount].isPoliceStation = 0;

        city[locationCount].head = NULL;

        locationCount++;
    }


    /* Special locations */

    city[1].isRailwayStation = 1;

    city[2].isAirport = 1;

    city[4].isHospital = 1;

    city[5].isPoliceStation = 1;


    /*
       Road format:

       source
       destination
       distance
       normal time
    */

    int roads[][4] =
    {
        {0, 1, 5, 10},
        {0, 3, 4, 8},
        {1, 7, 3, 6},
        {1, 6, 2, 5},
        {3, 7, 3, 7},
        {3, 9, 2, 5},
        {7, 4, 4, 8},
        {7, 8, 5, 10},
        {6, 2, 8, 15},
        {8, 2, 6, 12},
        {4, 5, 2, 4},
        {9, 2, 5, 10}
    };


    int roadCount =
        sizeof(roads) /
        sizeof(roads[0]);


    for (int i = 0;
         i < roadCount;
         i++)
    {
        int u = roads[i][0];
        int v = roads[i][1];

        int distance = roads[i][2];
        int time = roads[i][3];


        /* u -> v */

        Edge *e1 =
            (Edge *)malloc(
                sizeof(Edge)
            );

        e1->destination = v;
        e1->distance = distance;
        e1->time = time;
        e1->traffic = 1;
        e1->blocked = 0;

        e1->next =
            city[u].head;

        city[u].head = e1;


        /* v -> u */

        Edge *e2 =
            (Edge *)malloc(
                sizeof(Edge)
            );

        e2->destination = u;
        e2->distance = distance;
        e2->time = time;
        e2->traffic = 1;
        e2->blocked = 0;

        e2->next =
            city[v].head;

        city[v].head = e2;
    }
}


/* =========================================================
   FREE MEMORY
   ========================================================= */

void freeMemory()
{
    for (int i = 0;
         i < locationCount;
         i++)
    {
        Edge *current =
            city[i].head;

        while (current != NULL)
        {
            Edge *temp = current;

            current = current->next;

            free(temp);
        }

        city[i].head = NULL;
    }
}


/* =========================================================
   MENU
   ========================================================= */

void displayMenu()
{
    printf("\n\n");
    printf("=============================================\n");
    printf("        SMART CITY ROUTE FINDER\n");
    printf("=============================================\n");

    printf("1.  Display locations\n");
    printf("2.  Display roads\n");
    printf("3.  Find route\n");
    printf("4.  Compare routes\n");
    printf("5.  Add location\n");
    printf("6.  Add road\n");
    printf("7.  Update traffic\n");
    printf("8.  Block / Unblock road\n");
    printf("9.  Find nearest facility\n");
    printf("10. Emergency mode\n");
    printf("11. Search location\n");
    printf("12. Exit\n");

    printf("=============================================\n");
}


/* =========================================================
   MAIN
   ========================================================= */

int main()
{
    createSampleCity();

    int choice;

    printf("\n");
    printf("=============================================\n");
    printf("     WELCOME TO SMART CITY ROUTE FINDER\n");
    printf("=============================================\n");

    do
    {
        displayMenu();

        printf("\nEnter choice: ");

        scanf("%d", &choice);

        clearInputBuffer();


        switch (choice)
        {
            case 1:
                displayLocations();
                break;


            case 2:
                displayRoads();
                break;


            case 3:
                findRoute();
                break;


            case 4:
                compareRoutes();
                break;


            case 5:
                addLocation();
                break;


            case 6:
                addRoad();
                break;


            case 7:
                updateTraffic();
                break;


            case 8:
                toggleRoad();
                break;


            case 9:
                nearestFacility();
                break;


            case 10:
                emergencyMode();
                break;


            case 11:
                searchLocation();
                break;


            case 12:
                printf("\nThank you for using Smart City Route Finder!\n");
                break;


            default:
                printf("\nInvalid choice. Please try again.\n");
        }

    }
    while (choice != 12);


    freeMemory();

    return 0;
}
