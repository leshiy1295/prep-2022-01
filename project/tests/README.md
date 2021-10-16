### Запустить тесты

```bash
$ make clean && make
$ ./tests.out [./path/to/tests/data/dir/] [--with-extra]
```
Аргументы представляют собой:

- Путь до директории с данными для тестов (по умолчанию "./project/tests/data")

```bash
$ ls -l ./project/tests/data/
total 0
drwxr-xr-x  4 anthony  staff  136 Mar  3 21:21 test_element_setting
drwxr-xr-x  3 anthony  staff  102 Mar  4 15:47 test_matrices_mul
drwxr-xr-x  3 anthony  staff  102 Mar  4 15:47 test_matrices_sub
drwxr-xr-x  3 anthony  staff  102 Mar  4 15:48 test_matrices_sum
...

```
- Флаг запуска тестов на дополнительные операции над матрицей (det, adj, inv): --with-extra

```bash
$ ./tests.out
Testing of creating matrix from file....OK
Testing of element setting....OK
Testing of matrix arithmetic: sum, sub, mul.............OK
Testing of matrix scalar multiplication....OK
Testing of matrix transponse....OK
SUCCESS
```

```bash
./tests.out --with-extra
Testing of creating matrix from file....OK
Testing of element setting....OK
Testing of matrix arithmetic: sum, sub, mul.............OK
Testing of matrix scalar multiplication....OK
Testing of matrix transponse....OK
Extra:
Testing of getting matrix determinant.....OK
Test of getting agjugate matrix.....OK
Testing of matrix inversing.....OK
SUCCESS
```

### Проверить отсутствие утечек памяти

```bash
$ make clean && make
$ valgrind --tool=memcheck ./tests.out --with-extra
...
==22040== LEAK SUMMARY:
==22040==    definitely lost: 0 bytes in 0 blocks
==22040==    indirectly lost: 0 bytes in 0 blocks
...
==22040== 
==22040== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```
