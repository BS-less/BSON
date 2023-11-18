### TODO
- Replace assertions
- Some errors are not being handled
  - On error, successfully free everything, log what happened, and return proper error enum
### Current state
- With proper file
  - No memory leaks
  - No (discovered) crashes
- Fuzzed files
  - Potential leaks
  - Crashes
