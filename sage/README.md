# NTRU Implementation in SageMath

To run the known answer tests build the shared library for rng with `make`. Then in `sage`, run:
```python
load('kat.sage')
test_all()
```

This will show what tests pass and which tests fail. Additionally, it will show the average speed of the key gen, encapsulate, and decapsulate operations. 
