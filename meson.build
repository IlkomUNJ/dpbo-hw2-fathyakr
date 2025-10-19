project('my-cpp-project', 'cpp',
    version: '1.0.0',
    default_options: [
        'cpp_std=c++20',
        'warning_level=3',
        'werror=true', # Treat all warnings as errors
        'optimization=s' # Optimize for size
    ]
)

cpp_args = [
    # Recommended warnings for g++/clang
    '-Wpedantic',
    '-Wformat=2',
    '-Wundef',
    '-Wcast-align',
    '-Wwrite-strings',
    '-Wold-style-cast',
    '-Wnull-dereference',
    '-Wctor-dtor-privacy',
    '-Wno-missing-field-initializers',
]

add_global_arguments(cpp_args, language : 'cpp')

app_sources = [
    'main.cpp',
    'src/bank_customer.cpp',
    'src/order.cpp',
    'src/store.cpp',
    'src/buyer.cpp',
    'src/seller.cpp',
    'src/handlers.cpp',
]

hpp_dir = include_directories('include')

executable('store',
    app_sources,
    include_directories : hpp_dir,
    # Example dependencies:
    # dependencies: [fmt_dep],
    link_args: ['-static', '-static-libgcc', '-static-libstdc++'],
    install: true
)