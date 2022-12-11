//*************************************************
// File:   Set.h
// Author: Daniel Benedi Garcia
// Date:   8th December 2022
// Coms:   Set class that implements a skip list
//         and a lot of associated utilities
//*************************************************
#pragma once
#include <vector>

template <typename T>
class Set {
	public:
		/**
		 * Constructor.
		 *
		 * Creates an empty Set, sets the skip list parameter q to the
		 * given value
		 *
		 * @param q		double. Represents the probability of an element
		 * 				not having 
		 */
		Set(double q) : q(q) {}

		/**
		 * Destructor.
		 *
		 * Clear all the extra structures used.
		 */
		~Set(){
			Node* elem = head[0];
			while(elem != nullptr){
				Node* next = elem->forward[0];
				delete elem;
				elem = next;
			}
		}

		/**
		 * Insert element x in the set.
		 *
		 * @param x		Type depends on template. The element will be added
		 * 				to the Set.
		 */ 
		void insert(const T& x){
			if(contains(x)) return;
		
			int height = randomHeight();
			Node* new_elem = new Node(x, height);
			
			// If it is the first element in that height, add it to the head
			while(head.size() < new_elem->height){
				head.push_back(new_elem);
				height--;
			}
			height--;
			
			// It might happen that it is the fisrt one added
			if(height < 0) return;
			
			Node* tmp = head[height];
			while(height >= 0 && tmp->data > x){
				new_elem->forward[height] = tmp;
				tmp->backward[height] = new_elem;
				head[height] = new_elem;
				height--;
				if(height >= 0)
					tmp = head[height];
			}
			while(height >= 0){
				// While for that level there is a smaller element
				// and exist and following node, go forward
				while(tmp->forward[height] && tmp->forward[height]->data < x)
					tmp = tmp->forward[height];
				
				new_elem->backward[height] = tmp;
				if(tmp->forward[height]) {
					new_elem->forward[height] = tmp->forward[height];
					tmp->forward[height]->backward[height] = new_elem;
				}
				tmp->forward[height] = new_elem;
				height--;
			}
		}

		/**
		 * Removes the element x from the set if present.
		 * 
		 * @param x		Type depends on template. The element will be removed
		 * 				from the set.
		 */
		void remove(const T& x){
			Node* elem = search(x);
		
			if(elem && elem->data == x){
				for(int lvl = elem->height-1; lvl >= 0; lvl--){
					if(elem->backward[lvl]){
						elem->backward[lvl]->forward[lvl] = elem->forward[lvl];
						if(elem->forward[lvl]){
							elem->forward[lvl]->backward[lvl] = elem->backward[lvl];
						}
					}else{
						//If there is no previous element, that means it is in the head
						head[lvl] = elem->forward[lvl];
						if(!head[lvl]){
							while(lvl + 1 > head.size())
								head.pop_back();
						}
					}
				}
				delete elem;
			}
		}

		/**
		 * It will return wheter the element is in the set.
		 *
		 * @param x		Type depends on template. The element will be searched
		 * 				in the set.
		 * @return 		It will return true if the element is in the set, false
		 * 				otherwise.
		 */
		bool contains(const T& x) const{
			Node* res = search(x);
			return res && res->data == x;
		}

		/**
		 * Returns the total search cost.
		 *
		 * @return		It will return the toal search cost.
		 */
		int total_search_cost() const{
			int cost = 0;
			Node* next = head[0];
			while(next != nullptr){
				int height = head.size() - 1;
				while(height > 0 && head[height]->data > next->data){
					cost++;
					height--;
				}
			
				Node* res = head[height];
				while(height >= 0){
					cost++;
					if(res->forward[height] && res->forward[height]->data <= next->data){
						res = res->forward[height];
					}else{
						height--;
					}
				}
				
				next = next->forward[0];
			}

			return cost;
		}

		/**
		 * Returns the number of pointers used, that is the sum of theheight 
		 * of all nodes in the skip list.
		 * 
		 * @return		Total number of pointers.	
		 */
		int number_pointers() const{
			Node* next = head[0];
			int size = head.size();
			while(next != nullptr){
				size += next->height;
				next = next->forward[0];
			}
			
			return size;
		}

		double get_q() const {
			return q;
		}

		int nodes_lvl(int lvl) const {
			if(lvl >= head.size()) return 0;

			int nodes = 0;
			Node* node = head[lvl];
			while(node != nullptr){
				nodes++;
				node = node->forward[lvl];
			}
			
			return nodes;
		}

		friend std::ostream& operator<< (std::ostream& stream, const Set& set) {
			// Print HEAD
			for(int i = 0; i < set.head.size(); i++)
				stream << "--- "; stream << std::endl;
			for(int i = 0; i < set.head.size(); i++)
				stream << " Ø  "; stream << std::endl;
			for(int i = 0; i < set.head.size(); i++)
				stream << "--- "; stream << std::endl;

			// Print BODY
			Node* node = set.head[0];
			while(node != nullptr){
				for(int i = 0; i < set.head.size(); i++)
					stream << " |  "; stream << std::endl;
				for(int i = 0; i < node->height; i++)
					stream << " v  ";
				for(int i = node->height; i < set.head.size(); i++)
					stream << " |  "; stream << std::endl;
				for(int i = 0; i < node->height; i++) stream << "--- ";
				for(int i = node->height; i < set.head.size(); i++)
					stream << " |  "; stream << std::endl;
				for(int i = 0; i < node->height; i++)
					stream << " " << node->data << "  ";
				for(int i = node->height; i < set.head.size(); i++)
					stream << " |  "; stream << std::endl;
				for(int i = 0; i < node->height; i++)
					stream << "--- ";
				for(int i = node->height; i < set.head.size(); i++)
					stream << " |  "; stream << std::endl;

				node = node->forward[0];
			}

			// Print END
			for(int i = 0; i < set.head.size(); i++)
				stream << " |  "; stream << std::endl;
			for(int i = 0; i < set.head.size(); i++)
				stream << " v  "; stream << std::endl;
			for(int i = 0; i < set.head.size(); i++)
				stream << "--- "; stream << std::endl;
			for(int i = 0; i < set.head.size(); i++)
				stream << " ∞  "; stream << std::endl;
			for(int i = 0; i < set.head.size(); i++)
				stream << "--- "; stream << std::endl;

			return stream;
		}
	private:
		struct Node{
			const T& data;
			Node** forward;
			Node** backward;
			int height;
			Node(const T& data, int height) : data(data), height(height) {
				forward = new Node*[height];
				backward = new Node*[height];
			
				for(int i = 0; i < height; i++){
					forward[i] = nullptr;
					backward[i] = nullptr;
				}
			}

			~Node(){
				delete[] forward;
				delete[] backward;
			}
		};
		

		// data members
		std::vector<Node*> head; // First element for every height
		double q; // Probability of not having another level
		
		// Internal procedures
		Node* search(const T& x) const{
			int height = head.size()-1;
			while(height > 0 && head[height]->data > x) height--;
			if(height < 0) return nullptr;

			Node* res = head[height];
			while(height >= 0){
				if(res->forward[height] && res->forward[height]->data <= x){
					res = res->forward[height];
				}else{
					height--;
				}
			}
			
			return res;
		}

		int randomHeight(){
		    static std::random_device rd;
		    static std::mt19937 gen(rd());
		    static std::uniform_real_distribution<> dis(0.0,1.0);
			
			int height = 1;
			while(dis(gen) > q) height++;
		
			return height;
		}
}; 
