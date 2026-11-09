import inspect
import tinker

print("=== TINKER SDK INTROSPECTION ===" )
print(f"Package Path: {tinker.__file__}")
print("\n--- Available Attributes & Modules ---")
for name in dir(tinker):
    if not name.startswith('_'):
        attr = getattr(tinker, name)
        print(f"  - {name}: {type(attr)}")

print("\n--- Inspecting Sub-modules or Classes ---")
for name in dir(tinker):
    if not name.startswith('_'):
        attr = getattr(tinker, name)
        if inspect.isclass(attr) or inspect.ismodule(attr):
            print(f"\n[ Class/Module: {name} ]")
            for subname in dir(attr):
                if not subname.startswith('_'):
                    print(f"    . {subname}")
