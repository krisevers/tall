#include "../src/registry.h"
#include "../src/kernel.h"
#include "../src/types.h"
#include "../src/process.h"

#include <memory>
#include <string>


void print_tree(const tall::node_ptr& node, const std::string& prefix = "", bool is_last = true) {
    if (!node) return;

    std::cout << prefix;
    std::cout << (is_last ? "└── " : "├── ");

    // print node info based on type
    if (auto var_node = std::dynamic_pointer_cast<tall::variable>(node)) {
        std::cout << "VAR: " << var_node->id << std::endl;
    }
    else if (auto bin_node = std::dynamic_pointer_cast<tall::binary_op>(node)) {
        std::string op_str;
        switch(bin_node->operation) {
            case tall::ADD: op_str = "ADD (+)";     break;
            case tall::SUB: op_str = "SUB (-)";     break;
            case tall::MUL: op_str = "MUL (*)";     break;
            case tall::DIV: op_str = "DIV (/)";     break;
            case tall::EXP: op_str = "EXP (exp)";   break;
            case tall::SQR: op_str = "SQR (sqrt)";  break;
            case tall::POW: op_str = "POW (pow)";   break;
            default: op_str = "UNKNOWN";                  break;
        }
        std::cout << op_str << std::endl;

        // print children
        std::string child_prefix = prefix + (is_last ? "    " : "│   ");
        print_tree(bin_node->lhs, child_prefix, false);
        print_tree(bin_node->rhs, child_prefix, true);
    }
    else if (auto un_node = std::dynamic_pointer_cast<tall::unary_op>(node)) {
        std::string op_str;
        switch(un_node->operation) {
            case tall::ADD: op_str = "ADD (+)";     break;
            case tall::SUB: op_str = "SUB (-)";     break;
            case tall::MUL: op_str = "MUL (*)";     break;
            case tall::DIV: op_str = "DIV (/)";     break;
            case tall::EXP: op_str = "EXP (exp)";   break;
            case tall::SQR: op_str = "SQR (sqrt)";  break;
            case tall::POW: op_str = "POW (pow)";   break;
            default: op_str = "UNKNOWN";                  break;
        }
        std::cout << op_str << std::endl;
        std::string child_prefix = prefix + (is_last ? "    " : "│   ");
        print_tree(un_node->operand, child_prefix, true);
    }
}

int main()
{
   tall::registry r(10);

   // create variables
   auto x = r.var();
   auto y = r.var();
   auto z = r.var();

   // assemble into collections
   auto particles = r.assemble({x, y, z}, 1000);

   // hyperparameters
   auto sim_time    = r.par();
   auto dt          = r.par();

   auto sigma = r.par();
   auto beta  = r.par();
   auto rho   = r.par();
   tall::kernel lorenz(
       {
           x, x + (sigma*(y-x))  * dt;
           y, y + (x*(rho-z)-y)  * dt;
           z, z + (x*y - beta*y) * dt;
       }
   );

   auto a = r.par();
   auto b = r.par();
   auto c = r.par();
   tall::kernel rössler(
       {
           x, x + (- y - z)         * dt;
           y, y + (x + a * y)       * dt;
           z, z + (b + z * (x - c)) * dt;
       }
   );

   tall::process step;
   step.systems           = {lorenz, rössler};
   step.record            = {vars};
   step.record_interval   = 0;
   step.iterations        = static_cast<double>(sim_time/dt);
   step.alternate         = true;

   tall::process diffuse;

   tall::process pipeline;
   pipeline.systems = {step, diffuse};

   pipeline.evaluate(); // evaluate the pipeline one iteration into the future
   pipeline.profile();  // profile the pipeline to analysis optimization
   pipeline.execute();  // execute the pipeline and all sub-processes


   print_tree(dx.get_root());
   std::cout << std::endl;
   print_tree(dy.get_root());
   std::cout << std::endl;
   print_tree(dz.get_root());


    return 0;
}
