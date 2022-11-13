# Dynamic Array
## Usage
The DynamicArray was made to mimic 'vanilla' C++ arrays as closely as possible with more functions, operators and usage being made to closely link the usage of both. There are many limitations (things I have checked and couldn't find a way around such as the object creation having to be like standard objects (e.g. `DynamicArray<int> name = DynamicArray<int>();`) instead of the array object creation (e.g. `int name[] = {1,2,4,5};`).

---

### Object Creation
`DynamicArray<DataType> DynamicArrayName = DynamicArray<DataType>()` - Object Creation with default values (starting array size = 10, step size = 5) </br>
`DynamicArray<DataType> DynamicArrayName = DynamicArray<DataType>(int StartSize, int StepSize)` - object creation with custom values

#### Example
to create a char DynamicArray for chat logs, you would have to write `DynamicArray<char> ChatLogs = DynamicArray<char>();` and if you have a lot of ram and a lot of traffic, you would do `DynamicArray<char> ChatLogs = DynamicArray<char>(200, 100);` with your own values of course.

---

### Append
`DynamicArrayName.Append(ArrayDataType ObjectToAdd);`

#### Example
adding a charater to the chat logs from before would look like this `ChatLogs.Append('a');`

---

### MultiAppend
`DynamicArrayName.MultiAppend(ArrayDataType* beginning, int range)` - first parameter is a pointer to the first element of the array and second is the lenght that wants to get added </br>
`DynamicArrayName.MultiAppend(ArrayDataType* beginning, ArrayDataType* end)` - First parameter is a pointer to the first element of the array and then second pointer is a pointer to the last element

#### Example
if you want to append a char array `char SomeArray[6] = "hello";` then you would have to either do `ChatLogs.MultiAppend(&SomeArray[0], 6)` or `ChatLogs.MultiAppend(&SomeArray[0], &SomeArray[5])`. most c++ objects do have `.begin()` and `.end()` iterator functions which return pointers to the first and last element

---

### Replace
`DynamicArrayName.Replace(ArrayDataType ReplaceObject, int position);`

#### Example
if you wanna replace a character in the place 12 with x, you would write `ChatLogs.Replace('x', 12);`

---

### Remove
`DynamicArrayName.Remove(int position)` - Removes object in the position, and moves everything that was infront of it, back 1 spot to adjust

#### Example
If you wanna remove the character in position 50, you would write `ChatLogs.Remove(50);`

---
### Clear
`DynamicArrayName.Clear()` - Function which basically returns the object back to the state when it was first created (Starting array size and empty array)

---

### GetArray
`DynamicArrayName.GetArray()` - returns the array that is currently stored in the DynamicArray Object

### GetArraySize
`DynamicArrayName.GetArraySize()` - Returns The current Array Size

### GetArrayDefaultSize()
`DynamicArrayName.GetArrayDefaultSize()` - Returns the starting array size

### GetArrayIndexPointer
`DynamicArrayName.GetArrayIndexPointer()` - Returns the position of where the next object will be put (can be used for loops to go through the whole array)

### GetArrayStepSize
`DynamicArrayName.GetArrayStepSize()` - returns the step size, the amount the array increases when it reaches the limit

---

## Operators

`operator []` - Works like in normal arrays. returns a refrence to the object wanted </br>
`operator <<` - Inserts the DynamicArray on the right into the DynamicArray on the left </br>
`operator +=` - Works the same as the `operator <<`. Inserts the object on the right into the object on the left </br>
