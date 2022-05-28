# C++ Vector Class Recreation

Sukurta C++ vector klasė, pagal std::vector konteinerį.

## Kaip naudotis:

1. Parsisiųskite header failą.
2. Įtraukite failą į savo projektą:
`#include "Vector.h"
3. Naudokite taip pat, kaip naudotumėte std::vector, tačiau praleiskite std:: dalį.

## Funkcijos ir jų veikimas:

1. **Operator==** - patikrina ar abu vektoriai yra vienodo dydžio ir jų elementai sutampa.
```cpp
bool operator==(const vector<T>& v) const{
        if(size() != v.size())
            return false;
        for(size_type i = 0; i < size(); ++i)
            if(v[i] != (*this)[i])
                return false;
        return true;
    }
```
2. **Pop_back** - Pašalina paskutinį vektoriaus elementą.
```cpp
void pop_back(){
    iterator it = vSize;
    alloc.destroy(--it);
    vSize--;
    }
```
3. **Push_back** - Prideda elementą į vektoriaus galą.
```cpp
void push_back (const value_type& val){ 
        if(vCapacity == vSize)
            grow();
        alloc.construct(vSize++, val);
    }
```
4. **erase** - Pašalina elementą nurodytoje vietoje.
```cpp
iterator erase (iterator position){
        std::move(position+1, vSize, position);
        --vSize;
        return position;
    }
```
5. **shrink_to_fit** - Sumažina konteinerio talpą iki konteinerio dydžio.
```cpp
void shrink_to_fit(){
        if (vSize != vCapacity) {
            alloc.deallocate(vSize, vCapacity - vSize);
            vCapacity=vSize;
        }
    }
```

## Sukurto vektoriaus ir std::vector palyginimas

Vector elementų push_back palyginimas:

| Container\Size | std::vector | myVector |
|----------------|-------------|----------|
| 10000          | 0.000267    | 0.000111 |
| 100000         | 0.001956    | 0.000853 |
| 1000000        | 0.0172605   | 0.007066 |
| 10000000       | 0.177679    | 0.086794 |
| 100000000      | 1.760041    | 0.779684 |


Atminties perskirstymo kiekis ir pasiekta talpa  vektorių pripildžius su 100000000 elementų:

|   Container | Reallocation count | Reached capacity |
|------------:|:------------------:|------------------|
| std::vector |         28         |     134217728    |
|    myVector |         28         |     134217728    |

std::vector ir sukurto vector palyginimas v2.0 užduotyje:

|           Container | std::vector |  myVector |
|--------------------:|:-----------:|:---------:|
|     Failo skaitymas |   0.165175  |  0.145564 |
| Studentu rusiavimas |  0.0640473  | 0.0477812 |
|  Studentu isvedimas |   0.112016  |  0.112449 |
|    Programos laikas |   0.527748  |  0.451256 |