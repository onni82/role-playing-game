# pseudokod på engelska för vissa funktioner i projektuppgiften
 pseudokod för att kolla programmets argument. om det finns ett argument utöver programmets namn anger det andra argumentet spelarens namn. annars ber programmet efter spelarens namn:
 ```
 IF program arguments > 1 THEN
    player name = argument[1]
 ELSE player name = input from user
 END IF
 ```

 pseudokod förbinärsökningsfunktionen för structobjekt. returnerar indexet av objektet man söker efter om det finns, annars returneras -1:
 ```
 left = 0
 right = array size -1
 
 WHILE left <= right
    mid = left + (right - left) / 2

    IF array[mid] < key
        left = mid + 1
    ELSE IF array[mid] > key
        right = mid -1
    ELSE return mid
    END IF
 END WHILE
 ```

[README](README.md)