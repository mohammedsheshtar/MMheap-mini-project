#include <iostream>
#include <cctype>
#include <string>
#include <cstring>
#include <stdio.h>
#include <cstdlib>
#include <stdio.h>
#include <iomanip>
#include <cmath>
#include <list>
#include <algorithm>
#include <unordered_set>
using namespace std;


// Helper functions to find parent, left child, and right child indices
int Parent(int i) { return (i-1)/2; }
int LeftChild(int i) { return 2*i+1; }
int RightChild(int i) { return 2*i+2; }


void Heapify(int arr[], int n, int i, bool is_min_level) {
    int min_max_child = i;  // Initialize min_max_child as root
    int left = 2 * i + 1;  // left child
    int right = 2 * i + 2;  // right child

    // Find the minimum or maximum child based on the level
    if (is_min_level) {
        if (left < n && arr[left] < arr[min_max_child])
            min_max_child = left;
        if (right < n && arr[right] < arr[min_max_child])
            min_max_child = right;
    }
    else {
        if (left < n && arr[left] > arr[min_max_child])
            min_max_child = left;
        if (right < n && arr[right] > arr[min_max_child])
            min_max_child = right;
    }

    // If min_max_child is not the root
    if (min_max_child != i) {
        swap(arr[i], arr[min_max_child]);
 
        // If the node swapped is on a min level
        if (is_min_level) {
            // Compare the new value with its grandchildren and swap if necessary
            if (left < n && left + 1 < n) {
                if (arr[left + 1] < arr[min_max_child])
                    swap(arr[left + 1], arr[min_max_child]);
            }
            if (right < n && right + 1 < n) {
                if (arr[right + 1] < arr[min_max_child])
                    swap(arr[right + 1], arr[min_max_child]);
            }
            // Recursively heapify the affected sub-tree on max level
            Heapify(arr, n, min_max_child, false);
        }
        else {
            // Compare the new value with its grandchildren and swap if necessary
            if (left < n && left + 1 < n) {
                if (arr[left + 1] > arr[min_max_child])
                    swap(arr[left + 1], arr[min_max_child]);
            }
            if (right < n && right + 1 < n) {
                if (arr[right + 1] > arr[min_max_child])
                    swap(arr[right + 1], arr[min_max_child]);
            }
            // Recursively heapify the affected sub-tree on min level
            Heapify(arr, n, min_max_child, true);
        }
    }
}


class MMHeap {
public:
    int *A; // Array to store MM Heap
    int capacity; // Maximum size of MM Heap
    int size; // Current size of MM Heap
public:
    // Constructor
    MMHeap(int capacity) {
        this->capacity = capacity;
        this->size = 0;
        this->A = new int[capacity];
    }
    
    // Destructor
    ~MMHeap() {
        delete[] A;
    }
    
    // Build MM Heap from given array
    void BuildHeap(int arr[], int n) {
        // Copy elements from array to MM Heap
        for (int i = 0; i < n; i++) {
            A[i] = arr[i];
        }
        
        // Perform Heapify operation on all non-leaf nodes in reverse level order
        for (int i = Parent(n-1); i >= 0; i--) {
            Heapify(A, n, i, i%2 == 0);
        }
        
        // Update size of MM Heap
        size = n;
    }
    
// Return minimum priority value/key
    int Min() {
        return *min_element(A, A + sizeof(A));
    }
    
    // Return maximum priority value/key
    int Max() {
        return *max_element(A, A + sizeof(A));
    }
    
    
void Insert(int value) {
    if (size == capacity) {
        cout << "MM Heap is full." << endl;
        return;
    }
    
    // Insert new element at next available position in last level of MM Heap
    A[size] = value;
    size++;
    
    // Update mm-priority tree by swapping new element with its parent, grandparent, and great-grandparent (if necessary) until priority ordering is restored
    int i = size-1;
    while (i != 0) {
        int grandparent = Parent(Parent(i));
        int greatGrandparent = Parent(Parent(Parent(i)));
        if (i%2 == 0 && grandparent >= 0 && A[i] > A[grandparent]) {
            // Case 1: new element is a max element in a min level and is greater than its grandparent (a min element)
            swap(A[i], A[grandparent]);
            i = grandparent;
        } else if (i%2 != 0 && grandparent >= 0 && A[i] < A[grandparent]) {
            // Case 2: new element is a min element in a max level and is less than its grandparent (a max element)
            swap(A[i], A[grandparent]);
            i = grandparent;
        } else if (greatGrandparent >= 0 && ((i%2 == 0 && A[i] < A[greatGrandparent] && A[i] > A[Parent(i)]) || (i%2 != 0 && A[i] > A[greatGrandparent] && A[i] < A[Parent(i)]))) {
            // Case 3: new element is not a direct child of its grandparent and is out of order with its great-grandparent
            swap(A[i], A[greatGrandparent]);
            i = greatGrandparent;
        } else if (i%2 == 0 && A[i] < A[Parent(i)]) {
            // Case 4: new element is a max element in a min level and is less than its parent (a max element)
            swap(A[i], A[Parent(i)]);
            i = Parent(i);
        } else if (i%2 != 0 && A[i] > A[Parent(i)]) {
            // Case 5: new element is a min element in a max level and is greater than its parent (a min element)
            swap(A[i], A[Parent(i)]);
            i = Parent(i);
        } else {
            // Priority ordering has been restored
            break;
        }
    }
}

// Delete and return minimum priority value/key
int ExtractMin() {
    if (size == 0) {
        cout << "MM Heap is empty." << endl;
        return -1;
    }
    
    // Save minimum element and replace it with last element in MM Heap
    int min = A[0];
    A[0] = A[size-1];
    
    // Update mm-priority tree by performing Heapify operation on root
    Heapify(A, size-1, 0, true);
    
    // Update size of MM Heap
    size--;
    
    // Return minimum element
    return min;
}

// Delete and return maximum priority value/key
int ExtractMax() {
    if (size <= 1) {
        cout << "MM Heap is empty or has only one element." << endl;
        return -1;
    }
    
    // Save maximum element and replace it with last element in second last level of MM Heap
    int max = A[1];
    A[1] = A[size-1];
    
    // Update mm-priority tree by performing Heapify operation on root
    Heapify(A, size-1, 1, false);
    
    // Update size of MM Heap
    size--;
    
    // Return maximum element
    return max;
    }
};

int main() {
    // Test MM Heap operations
    int arr[] = {3, 7, 2, 4, 8, 5, 1, 6};
    int n = sizeof(arr)/sizeof(arr[0]);
    MMHeap mh(n);
    mh.BuildHeap(arr, n);

    //print current array
    cout << "current array: ";
        for(int i = 0; i < n;i++) {
            cout << arr[i] << " ";
        }

    cout << endl;
    cout << endl;

    cout << "heap before extracting the min and max priotity values:-" << endl << endl;
    
    cout << "initial heap: ";
    for (int i = 0; i < mh.size; i++) {
        cout << mh.A[i] << " ";
    }
    
    cout << endl;
    
    cout << "Maximum priority value/key: " << mh.Max() << endl;
    cout << "Minimum priority value/key: " << mh.Min() << endl;
    
  
    cout << endl;
    cout << endl;

    //extract the min and max values to test if our heap reorders the values correctly
    cout << "Extracting maximum priority values: " << mh.ExtractMax() << endl;
    cout << "Extracting minimum priority values: " << mh.ExtractMin() << endl;
 

    cout << endl;

    //print the heap along with the min and max to see if the heap is reordered correctly
    cout << "heap after extracting the min and max priotity values:-" << endl;
    
    cout << "current heap: ";
    for (int i = 0; i < mh.size; i++) {
     cout << mh.A[i] << " ";
    }
    
    cout << endl;
    
    cout << "Maximum priority value/key: " << mh.Max() << endl;
    cout << "Minimum priority value/key: " << mh.Min() << endl;
    
    


    cout << endl;
    
    //insert new values to the heap to test of the reordering in insert() is correct
    mh.Insert(9);
    mh.Insert(0);



    // print the final heap along with the min and max
    cout << "heap after inserting zero and nine:-" << endl << endl;
    
    cout << "final heap: ";
    for (int i = 0; i < mh.size; i++) {
        cout << mh.A[i] << " ";
    }
        
    cout << endl;
    
    cout << "Maximum priority value/key: " << mh.Max() << endl;
    cout << "Minimum priority value/key: " << mh.Min() << endl;
    

    return 0;

}