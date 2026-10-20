#ifndef list
#define list

template <typename T>
class List {
public:

    List() {
        head = nullptr;
        tail = nullptr;
    }

    List(const List<T>& list) = delete;
    List<T>& operator = (const List<T>& list) = delete;

    void addFirst(T *data) {
        Elem *elem = new Elem(data, head);
        head = elem;
        if (tail == nullptr) tail = head;
    }

    void addLast(T *data) {
        Elem *elem = new Elem(data, 0);
        if (tail) {
            tail->next = elem;
            tail = elem;
        } else {
            head = tail = elem;
        }
    }

    T* removeFirst() {
        if (head == nullptr) return nullptr;
        Elem *elem = head;
        head = head->next;
        if (head == nullptr) tail = nullptr;
        T *ret = elem->data;
        delete elem;
        return ret;
    }

    T* peekFirst() {
        if (head == nullptr) return nullptr;
        return head->data;
    }

    T* removeLast() {
        if (head == nullptr) return nullptr;
        Elem* prev = nullptr;
        for (Elem* curr = head; curr && curr != tail; curr = curr->next) {
            prev = curr;
        }
        Elem *elem = tail;
        if (prev != nullptr) prev->next = 0;
        else head = nullptr;
        tail = prev;
        T *ret = elem->data;
        delete elem;
        return ret;
    }

    T* peekLast() {
        if (tail == nullptr) return 0;
        return tail->data;
    }

private:

    struct Elem {
        T* data;
        Elem* next;
        Elem(T* d, Elem* n) {
            data = d;
            next = n;
        }
    };

    Elem *head, *tail;
};

#endif
