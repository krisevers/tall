#include "registry.h"

tall::registry(std::size_t capacity) : capacity(capacity) {
    size = 0;
    versions = tall::tensor<OPS>(capacity);
    relation = tall::tensor<int>(capacity, capacity);
}

ID tall::registry::add(OPS type) {
    ID newid;
    versions[newid] = type;
    size++;
    return newid;
}

bool tall::registry::has(ID id) {
    return id < capacity && versions[id] != INACTIVE;
}

tall::RESULT tall::registry::give(ID parent, ID child, int copies) {
    if (!has(parent)) {
        tall::log::ERROR("parent not initialized...\n");
        return FAILURE;
    }
    if (!has(child)) {
        tall::log::ERROR("child not initialized...\n");
        return FAILURE;
    }
    if (has_parent(relations, parent, child)) {
        tall::log::ERROR("child is an ancestor of parent...\n");
        return FAILURE;
    }
    if (parent == child) {
        tall::log::ERROR("not allowed to make circular hierarchies...\n");
        return FAILURE;
    }
    relations[parent, child] = copies;
    return SUCCESS;
}

tall::node tall::registry::var() {
    ID id = add(var);
    return variable(id);
}

tall::node tall::registry::par() {
    ID id = add(PAR);
    return parameter(id);
}

tall::node tall::registry::assemble(std::initializer_list<node> components, std::size_t copies=1) {
    ID p = add(COL);
    for (auto c : components) {
        auto var = std::dynamic_pointer_cast<tall::variable>(c.get_root());
        if (!give(p, var->id, copies)) {
            tall::log::DEBUG("assembling failed...\n");
        };
    }
    return collection(p);
}





#ifdef TEST_REGISTRY

#include <thread>
#include <vector>

void work()
{
    std::ostringstream oss; oss << "Thread " << std::this_thread::get_id() << " started.";
    std::string id = oss.str();

    for (int i = 0; i < 10; i++)
    {
        tall::log::INFO(id + " working...");
        tall::log::WARNING(id + " warning...");
        tall::log::ERROR(id + " error...");
        tall::log::DEBUG(id + " debug...");
    }

}

int main()
{

    // tall::log::configure({{"type", "file"}, {"filename", "test"}});

    std::vector<std::shared_ptr<std::thread>> threads(std::thread::hardware_concurrency());
    for (auto &thread : threads)
    {
        thread.reset(new std::thread(work));
    }

    for (auto &thread : threads)
    {
        thread->join();
    }


    return 0;
}
#endif // TEST_REGISTRY
