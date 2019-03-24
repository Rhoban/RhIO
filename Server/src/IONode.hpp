#ifndef RHIO_IONODE_HPP
#define RHIO_IONODE_HPP

#include <map>
#include <vector>
#include <string>
#include <mutex>
#include "ValueNode.hpp"
#include "CommandNode.hpp"
#include "StreamNode.hpp"
#include "FrameNode.hpp"

namespace RhIO
{
/**
 * Tree name separator
 */
constexpr char separator = '/';

/**
 * IONode
 *
 * Main class representing an Node in
 * virtual input/output hierarchy name tree
 * with children Nodes
 */
class IONode final : public ValueNode, public CommandNode, public StreamNode, public FrameNode
{
public:
  /**
   * Empty initialization
   */
  IONode();

  /**
   * Initialization with node name and parent
   */
  IONode(const std::string& name, IONode* parent);

  /**
   * Destructor
   */
  ~IONode();

  /**
   * Return Node name
   */
  const std::string& name() const;

  /**
   * Return absolute node name
   */
  const std::string& pwd() const;

  /**
   * Return Node parent and return self
   * if this is the root
   */
  const IONode& parent() const;
  IONode& parent();

  /**
   * Return Root Node
   */
  const IONode& root() const;
  IONode& root();

  /**
   * Return true if given subnode exist
   */
  bool childExist(const std::string& name) const;

  /**
   * Find and return child Node with given relative name
   * Throw logic_error exception if the asked node name
   * does not exist
   */
  const IONode& child(const std::string& name) const;
  IONode& child(const std::string& name);

  /**
   * Create a new child or complete branch with
   * given relative name
   * Do nothing if given hierarchy already exist
   */
  void newChild(const std::string& name);

  /**
   * Return a list of Node children relative names
   */
  std::vector<std::string> listChildren() const;

  /**
   * Save recursively the subtree into given
   * path directory
   */
  void save(const std::string& path);

  /**
   * Load recursively the subtree into given
   * path directory
   */
  void load(const std::string& path);

private:
  /**
   * Node name
   */
  std::string _name;

  /**
   * Node absolute name
   */
  std::string _pwd;

  /**
   * Pointer to parent Node
   * nullptr if root
   */
  IONode* _parent;

  /**
   * Children Nodes container pointer
   * (pointer to prevent address change when
   * container update)
   */
  std::map<std::string, IONode*> _children;

  /**
   * Mutex protecting concurent branch children
   * modification
   */
  mutable std::mutex _mutex;

  /**
   * Copy and assignment operator
   * should not called by user.
   * (Update BaseNode forwardFunc)
   */
  IONode(const IONode& node);
  IONode& operator=(const IONode& node);

  /**
   * If given name is referring to this Node, nullptr
   * is returned.
   * If given name is referring to a child Node, a pointer
   * to this Node is returned and newName is set to the
   * given name converted relatively to returned Node.
   * If given name is invalid, throw logic_error exception.
   * unless createBranch is true. Missing Nodes are then
   * created.
   */
  IONode* forwardChildren(const std::string& name, std::string& newName, bool createBranch);
};

}  // namespace RhIO

#endif
