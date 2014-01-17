#ifndef BINOMIAL_HEAP_H
#define BINOMIAL_HEAP_H

#include "hasvalue.hpp"
#include "node.hpp"

#include "avlnode.hpp"
#include <functional>
#include "countable.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

namespace tf{
template <class Node>
struct fibonacci_heap_node_base : public flink<Node>
{
    size_t h;// degree, from 0
    bool marked; 

    fibonacci_heap_node_base():h(0), marked(false){}
};
template <class Value>
struct fibonacci_heap_node: public fibonacci_heap_node_base<fibonacci_heap_node<Value> > ,
                 public has_value<Value>
{
    fibonacci_heap_node(const Value& value = Value()) : has_value<Value>(value) {}
    std::string valueToString() // implement
    {
        return "(" + to_string(this->v) + ", " + 
			to_string(this->h) + ", " + 
			(this->marked ? "t" : "f") + ")";
    }
};
template <class Value, class Cmp = std::less<Value> >  
class fibonacci_heap : public countable
{
public:
    typedef fibonacci_heap_node<Value> node_t;
    typedef node_t* link_t;

    /*
        size_T cnt
        */
    Cmp cmp;
    link_t head;
    link_t pmin;

	fibonacci_heap() : countable(0),
		                head(new node_t()),
	                    pmin(0)
	{
        head->l = head->r = head;
        head->p = head->d = 0;
	}

    bool empty()
	{
        return cnt == 0;
	}
    // 清除所有结点
    void destroy(link_t pnode)
	{
        if (pnode == 0) return;
		else
		{
            link_t d = down(pnode);
            if (d == 0)// has no childern
			{
                if (right(pnode) == pnode)// only one node in this level
				{
                    delete pnode;
				}
				else// two or more in this level
				{
                    link_t next = right(pnode);
                    left(pnode)->r = right(pnode);
                    right(pnode)->l = left(pnode);
                    delete pnode;
                    destroy(next);
				}
			}
			else// has children
			{
                destroy(d);
                d = 0;
                destroy(pnode);
			}
		}
	}
    ~fibonacci_heap()
	{
        cnt = 0;
        destroy(down(head));
    }
    // 插入一个新结点到第一层
    link_t insertOneNode(const Value& value)
	{
        link_t pnode = new node_t(value);
        insertOneNodeInFirstLevel(pnode, 1);
        return pnode;
	}
    // 为p结点插入一个孩子结点，孩子结点可能也有它的孩子
    link_t insertOneNodeAsChild(link_t p, link_t pnode, size_t child_cnt)
	{
        //std::cout << "insertOneNodeAsChild p = " << p->valueToString() << " pnode = "<< pnode->valueToString() << std::endl;

        if (height(p) == 0)// p无孩子
		{
            p->d = pnode;
            pnode->p = p;

            pnode->l = pnode->r = pnode;
            //pnode->d = 0;// pnode可能有孩子
		}
		else//这至少有一个孩子结点
		{
            link_t pos = down(p);// 插入到第一个孩子之后
            link_t next = right(pos);

            pnode->l = pos;
            pos->r = pnode;

            pnode->r = next;
            next->l = pnode;

            pnode->p = p;
            p->d = pnode;// 新插入到孩子变成第一个孩子结点
		}
        ++p->h; // 增加p的度
        cnt += child_cnt;// 总结点数 累计
        // 若新插入的结点在第一层，则可能要更改最小值
        if (p == head)
		{
            if (height(p) == 1) pmin = pnode;// 新插入的结点是唯一的结点
			else
			{
               if (cmp(pnode->v , pmin->v)) pmin = pnode;
			}
		}        
        return pnode;
	}
    // 在第一层插入结点
    link_t insertOneNodeInFirstLevel(link_t pnode, size_t child_cnt)
	{
        //std::cout << "insertOneNodeInFirstLevel" << std::endl;
		return insertOneNodeAsChild(head, pnode, child_cnt);
	}
    // 是否是第一个结点
    bool isFirstNodeInThisLevel(link_t pnode)
	{
        return pnode == down(parent(pnode));
	}
    void treeShape()
	{
        treeShape(head->d, 0, false, head->d);
	}
    void treeShape(link_t pnode, size_t table_cnt = 0, bool is_check = false , link_t check_link = 0)
	{
//std::cout << "treeShape cnt = " <<  std::endl;
        std::string delimiter = std::string(table_cnt, '\t');
        if (pnode == 0) std::cout << delimiter << "[None]" << std::endl;
        else
        {
            if (is_check && pnode == check_link) return;
			else
			{
                std::cout << delimiter + pnode->valueToString() << std::endl;
                treeShape(down(pnode), table_cnt + 1, false, down(pnode));
                treeShape(right(pnode), table_cnt, true, check_link);
			}
        }
	}
    void display()
	{
        std::cout << "-----------------------------" << std::endl;
        std::cout << "size = " << cnt << std::endl;
		std::cout << "minimum = " << (pmin ? pmin->valueToString() : "[None]") << std::endl;
		std::cout << "head = " << head->valueToString() << std::endl;
        treeShape();
	}
    void size() const
	{
        return cnt;
	}
    struct degree_table : public std::vector<link_t>
	{
        bool hasOne(size_t degree)
		{
            if (degree >= size())
			{
                resize(degree + 1);
                return false;
			}
            return at(degree) != 0;
		}
        
	};
    // 从双向链表中删除一个结点
    void removeOneNodeNormal(link_t pnode)
	{
        if (height(parent(pnode)) > 1)// 本层大于一个结点
		{
            link_t next = right(pnode);
            link_t prev = left(pnode);

            parent(pnode)->d = next;

            next->l = prev;
            prev->r = next;
		}
		else// 第一层只有一个结点
		{
            parent(pnode)->d = 0;
		}
        --parent(pnode)->h;//度减少1
	}
    // 删除第一层一个结点，不更改总结点个数
    void removeOneNodeNormalInFirstLevel(link_t pnode)
	{
		//std::cout << "removeOneNodeNormalInFirstLevel pnode = " << pnode->valueToString() << std::endl;
       removeOneNodeNormal(pnode);
	}
    // 把两个度相同的结点链接到一起，返回新根结点
    link_t linkSameDegree(link_t x, link_t y)
	{
        //std::cout << "linkSameDegree x = " << x->valueToString() << "  y = " << y->valueToString() << std::endl;

        if (cmp(y->v, x->v)) std::swap(x, y);// x<y , x为根
        removeOneNodeNormalInFirstLevel(y);
		insertOneNodeAsChild(x, y, 0);
        return x;
	}
    void mininum() 
	{
        return pmin;
	}
    // extract 操作执行后，孩子加入
    void addListToFirstLevel(link_t first, size_t child_cnt)
	{
        if (first == 0) return;
		else
		{
            link_t p = first;
            while (parent(p) != head)
			{
                p->p = head;
                p = right(p);
			}

            if (height(head) == 0)
			{
                head->d = first;
			}
			else
			{
                link_t pos = down(head);
                link_t next = right(pos);

                link_t first_l = left(first);
                // pos (first first_l) next
                pos->r = first;
                first->l = pos;

                first_l->r = next;
                next->l = first_l;
			}
            head->h += child_cnt;
		}
	}
    // 删除最小结点，总个数减少1
    void extractMinimum()
	{
        if (height(head) == 0) return;
		else
		{
			deleteOneNodeInFirstLevelAndConsolidate(pmin);
		}
	}
    // 合并相同度的结点
    void consolidate()
	{
        if (height(head) <= 1) 
		{
            pmin = down(head);
		}
		else// 第一层结点个数大于1，可能要合并
		{
            link_t first = down(head);// 从第一个结点处合并、此时pmin刚被抽走

            link_t pnode = first;
            degree_table table;
            int n = height(head);// 最多合并n - 1次,但迭代2n-2次,此时n个结点：1，1，2，3，4.。
            int m = 2*n - 2;
            while (m--)
			{
                //std::cout << "pnode = " << pnode->valueToString() << std::endl;
                //display();
                if (table.hasOne(height(pnode)))// 有相同度的结点another,合并下次迭代
				{// merge
                    link_t another = table.at(height(pnode));
                    if (pnode != another)
					{
                        table[height(another)] = 0;// 清除
                        link_t merged = linkSameDegree(pnode, another);

                        pnode = merged;// 转入下次迭代，有可能连续合并
					}

				}
				else// 没有相同度的结点，记录此度，转下一个结点
				{
                    table[height(pnode)] = pnode;
                    pnode = right(pnode);
				}
			}
           // 计算最小结点
            link_t firstp = down(head);
            pmin = down(head);
            for (link_t p = right(firstp); p !=  firstp; p = right(p))
			{
                if (cmp(p->v, pmin->v)) pmin = p;
			}
		}
        //
	}
    void decreaseKey(link_t pnode, const Value& value)
	{
        if (pnode == 0 || !cmp(value, pnode->v)) return;
		else// pnode有效，并且值变小
		{
            pnode->v = value;
            link_t par = parent(pnode);

            if (par == head)// pnode在第一层
            {
                if (cmp(value, pmin->v)) pmin = pnode;// 新值更小
            }
            else if (cmp(value, par->v))//非第一层，新值比父结点还小
            {
                cut(pnode, par);
            }
		}

	}
    // 切断pnode与p, p不为head
    void cut(link_t pnode, link_t p)
	{
		do// 切割（pnode, p), 若p不为第一层并且已经标记，则迭代
		{
            removeOneNodeNormal(pnode);
		    insertOneNodeInFirstLevel(pnode, 0);

            pnode->marked = false;

            pnode = p;
            p = parent(pnode);
		} while (p != head && pnode->marked);// 终止 p为head 或 p未标记

        if (p != head)
		{
            pnode->marked = true;
		}
	}
    void deleteOneNode(link_t pnode)
	{
        link_t par = parent(pnode);
        if (par == head)//被删除结点在第一层
		{
            if (pnode == pmin)
			{
                extractMinimum();
			}
			else
			{
                deleteOneNodeInFirstLevelAndConsolidate(pnode);
			}
		}
		else
		{
            cut(pnode, par);
            // 此进pnode位于第一层
			deleteOneNodeInFirstLevelAndConsolidate(pnode);
		}
	}
    void deleteOneNodeInFirstLevelAndConsolidate(link_t pnode)
	{
        link_t e = pnode;
        removeOneNodeNormalInFirstLevel(e);
        --cnt;
        // e的所有孩子加入到 第一层，执行合并
        addListToFirstLevel(down(e), height(e));
        delete e;
        consolidate();
	}
    void mergeWithAnother(fibonacci_heap<Value>& another)
	{
		if (another.empty()) return;
		if (cmp(another.pmin->v, pmin->v)) pmin = another.pmin;
		cnt += another.cnt;
		addListToFirstLevel(down(another.head), height(another.head));

		another.head->d = 0;
		another.head->h = 0;
        another.cnt = 0;
		another.pmin = 0;
	}
};

}
#endif