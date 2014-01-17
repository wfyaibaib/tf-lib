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
    // ������н��
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
    // ����һ���½�㵽��һ��
    link_t insertOneNode(const Value& value)
	{
        link_t pnode = new node_t(value);
        insertOneNodeInFirstLevel(pnode, 1);
        return pnode;
	}
    // Ϊp������һ�����ӽ�㣬���ӽ�����Ҳ�����ĺ���
    link_t insertOneNodeAsChild(link_t p, link_t pnode, size_t child_cnt)
	{
        //std::cout << "insertOneNodeAsChild p = " << p->valueToString() << " pnode = "<< pnode->valueToString() << std::endl;

        if (height(p) == 0)// p�޺���
		{
            p->d = pnode;
            pnode->p = p;

            pnode->l = pnode->r = pnode;
            //pnode->d = 0;// pnode�����к���
		}
		else//��������һ�����ӽ��
		{
            link_t pos = down(p);// ���뵽��һ������֮��
            link_t next = right(pos);

            pnode->l = pos;
            pos->r = pnode;

            pnode->r = next;
            next->l = pnode;

            pnode->p = p;
            p->d = pnode;// �²��뵽���ӱ�ɵ�һ�����ӽ��
		}
        ++p->h; // ����p�Ķ�
        cnt += child_cnt;// �ܽ���� �ۼ�
        // ���²���Ľ���ڵ�һ�㣬�����Ҫ������Сֵ
        if (p == head)
		{
            if (height(p) == 1) pmin = pnode;// �²���Ľ����Ψһ�Ľ��
			else
			{
               if (cmp(pnode->v , pmin->v)) pmin = pnode;
			}
		}        
        return pnode;
	}
    // �ڵ�һ�������
    link_t insertOneNodeInFirstLevel(link_t pnode, size_t child_cnt)
	{
        //std::cout << "insertOneNodeInFirstLevel" << std::endl;
		return insertOneNodeAsChild(head, pnode, child_cnt);
	}
    // �Ƿ��ǵ�һ�����
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
    // ��˫��������ɾ��һ�����
    void removeOneNodeNormal(link_t pnode)
	{
        if (height(parent(pnode)) > 1)// �������һ�����
		{
            link_t next = right(pnode);
            link_t prev = left(pnode);

            parent(pnode)->d = next;

            next->l = prev;
            prev->r = next;
		}
		else// ��һ��ֻ��һ�����
		{
            parent(pnode)->d = 0;
		}
        --parent(pnode)->h;//�ȼ���1
	}
    // ɾ����һ��һ����㣬�������ܽ�����
    void removeOneNodeNormalInFirstLevel(link_t pnode)
	{
		//std::cout << "removeOneNodeNormalInFirstLevel pnode = " << pnode->valueToString() << std::endl;
       removeOneNodeNormal(pnode);
	}
    // ����������ͬ�Ľ�����ӵ�һ�𣬷����¸����
    link_t linkSameDegree(link_t x, link_t y)
	{
        //std::cout << "linkSameDegree x = " << x->valueToString() << "  y = " << y->valueToString() << std::endl;

        if (cmp(y->v, x->v)) std::swap(x, y);// x<y , xΪ��
        removeOneNodeNormalInFirstLevel(y);
		insertOneNodeAsChild(x, y, 0);
        return x;
	}
    void mininum() 
	{
        return pmin;
	}
    // extract ����ִ�к󣬺��Ӽ���
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
    // ɾ����С��㣬�ܸ�������1
    void extractMinimum()
	{
        if (height(head) == 0) return;
		else
		{
			deleteOneNodeInFirstLevelAndConsolidate(pmin);
		}
	}
    // �ϲ���ͬ�ȵĽ��
    void consolidate()
	{
        if (height(head) <= 1) 
		{
            pmin = down(head);
		}
		else// ��һ�����������1������Ҫ�ϲ�
		{
            link_t first = down(head);// �ӵ�һ����㴦�ϲ�����ʱpmin�ձ�����

            link_t pnode = first;
            degree_table table;
            int n = height(head);// ���ϲ�n - 1��,������2n-2��,��ʱn����㣺1��1��2��3��4.��
            int m = 2*n - 2;
            while (m--)
			{
                //std::cout << "pnode = " << pnode->valueToString() << std::endl;
                //display();
                if (table.hasOne(height(pnode)))// ����ͬ�ȵĽ��another,�ϲ��´ε���
				{// merge
                    link_t another = table.at(height(pnode));
                    if (pnode != another)
					{
                        table[height(another)] = 0;// ���
                        link_t merged = linkSameDegree(pnode, another);

                        pnode = merged;// ת���´ε������п��������ϲ�
					}

				}
				else// û����ͬ�ȵĽ�㣬��¼�˶ȣ�ת��һ�����
				{
                    table[height(pnode)] = pnode;
                    pnode = right(pnode);
				}
			}
           // ������С���
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
		else// pnode��Ч������ֵ��С
		{
            pnode->v = value;
            link_t par = parent(pnode);

            if (par == head)// pnode�ڵ�һ��
            {
                if (cmp(value, pmin->v)) pmin = pnode;// ��ֵ��С
            }
            else if (cmp(value, par->v))//�ǵ�һ�㣬��ֵ�ȸ���㻹С
            {
                cut(pnode, par);
            }
		}

	}
    // �ж�pnode��p, p��Ϊhead
    void cut(link_t pnode, link_t p)
	{
		do// �иpnode, p), ��p��Ϊ��һ�㲢���Ѿ���ǣ������
		{
            removeOneNodeNormal(pnode);
		    insertOneNodeInFirstLevel(pnode, 0);

            pnode->marked = false;

            pnode = p;
            p = parent(pnode);
		} while (p != head && pnode->marked);// ��ֹ pΪhead �� pδ���

        if (p != head)
		{
            pnode->marked = true;
		}
	}
    void deleteOneNode(link_t pnode)
	{
        link_t par = parent(pnode);
        if (par == head)//��ɾ������ڵ�һ��
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
            // �˽�pnodeλ�ڵ�һ��
			deleteOneNodeInFirstLevelAndConsolidate(pnode);
		}
	}
    void deleteOneNodeInFirstLevelAndConsolidate(link_t pnode)
	{
        link_t e = pnode;
        removeOneNodeNormalInFirstLevel(e);
        --cnt;
        // e�����к��Ӽ��뵽 ��һ�㣬ִ�кϲ�
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