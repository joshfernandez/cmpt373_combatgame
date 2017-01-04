
add_library(expr-evaluator
        ../../include/expr-evaluator/ExprEvaluator.hpp
        ExprEvaluator.cpp
        )

target_link_libraries(expr-evaluator glog)

set_target_properties(expr-evaluator
        PROPERTIES
        LINKER_LANGUAGE CXX
        CXX_STANDARD 14
        )

install(TARGETS expr-evaluator
        ARCHIVE DESTINATION lib
        )

