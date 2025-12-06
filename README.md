# earley

## Build
```
mkdir build && cd build
cmake -DENABLE_COVERAGE=ON ..
make
make coverage  # запустит тесты и создаст отчет
```

## Testing
```
# мои тесты
make coverage

# свои тесты
./parser
<Grammar>
<word>
```
