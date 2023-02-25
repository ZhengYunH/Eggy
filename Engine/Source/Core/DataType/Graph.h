#pragma once
#include "Core/Config.h"

namespace Eggy
{
	template<typename T>
	struct TGraph;

	template<typename T>
	struct TEdge;

	template<typename T>
	struct TNode
	{
		using Graph = TGraph<T>;
		using Edge = TEdge<T>;

		TNode(Graph* graph, T& object) : mGraph_(graph), mObject_(object)
		{
		}

		
		void Link(TNode* node)
		{
			Edge* edge = mGraph_->AddEdge();
			edge->mSrcNode = this;
			edge->mDstNode = node;

			this->mOutEdges_.push_back(edge);
			node->mInEdges_.push_back(edge);
		}

		void Unlink(TNode* node)
		{
			List<Edge*> toRemoves;
			for (Edge* edge : mInEdges_)
			{
				if (edge->mSrcNode == node)
				{
					toRemoves.push_back(edge);
				}
			}

			for (Edge* edge : mOutEdges_)
			{
				if (edge->mDstNode == node)
				{
					toRemoves.push_back(edge);
				}
			}

			for (Edge* edge : toRemoves)
			{
				if (edge->mSrcNode == this)
				{
					mInEdges_.remove(edge);
					mGraph_->RemoveEdge(edge);
				}
				else
				{
					mOutEdges_.remove(edge);
					mGraph_->RemoveEdge(edge);
				}
			}
		}


		T& mObject_;
		List<Edge*> mInEdges_;
		List<Edge*> mOutEdges_;
		Graph* mGraph_;
	};

	template<typename T>
	struct TEdge
	{
		using Node = TNode<T>;
		using Graph = TGraph<T>;

		Node* mSrcNode;
		Node* mDstNode;
	};

	template<typename T>
	struct TGraph
	{
		using Node = TNode<T>;
		using Edge = TEdge<T>;

		~TGraph()
		{
			for (auto node : mNodes)
				SafeDestroy(node);

			for (auto edge : mEdges)
				SafeDestroy(edge);
		}

		Node* AddNode(T& object) 
		{ 
			Node* node = new Node(this, object); 
			mNodes.emplace_back(node); 
			return node;
		}

		void RemoveNode(Node* node)
		{
			if (auto itr = std::find(mNodes.begin(), mNodes.end(), node) != mNodes.end())
			{
				delete *itr;
				mNodes.erase(itr);
			}
		}
		
		Edge* AddEdge()
		{
			Edge* edge = new Edge();
			return edge;
		}

		void RemoveEdge(Edge* edge)
		{
			if (auto itr = std::find(mEdges.begin(), mEdges.end(), edge) != mEdges.end())
			{
				delete* itr;
				mEdges.erase(itr);
			}
		}

		bool GetDependencyList(List<Node*>& outNodes)
		{
			List<uint8> tags(mNodes.size(), 0);
			List<size_t> searchStack;
			bool isLegal = true;

			List<Node*> endNodes;

			for (Node* node : mNodes)
			{
				if (node->mOutEdges_.empty())
					endNodes.push_back(node);
			}

			for (Node* node : endNodes)
			{
				searchStack.clear();
				if (!RecursiveFindParent(outNodes, tags, searchStack, GetNodeIndex(node)))
					return false;
			}
			return true;
		}

	private:
		size_t GetNodeIndex(Node* node)
		{
			auto itr = std::find(mNodes.begin(), mNodes.end(), node);
			HYBRID_CHECK(itr != mNodes.end());
			return std::distance(mNodes.begin(), itr);
		}

		bool RecursiveFindParent(List<Node*>& outNodes, List<uint8>& tags, List<size_t>& searchStack, size_t index)
		{
			if (!tags[index])
			{
				if (mNodes[index]->mInEdges_.empty())
				{
					outNodes.push_back(mNodes[index]);
				}
				else
				{
					searchStack.push_back(index);
					for (Edge* edge : mNodes[index]->mInEdges_)
					{
						auto parentNode = edge->mSrcNode;
						size_t parentIndex = GetNodeIndex(parentNode);
						// exist loop
						if (std::find(searchStack.begin(), searchStack.end(), parentIndex) != searchStack.end())
							return false;
						if (!RecursiveFindParent(outNodes, tags, searchStack, parentIndex))
							return false;
					}
					searchStack.pop_back();
					outNodes.push_back(mNodes[index]);
				}
			}
			tags[index] = 1;
			return true;
		}

		List<Node*> mNodes;
		List<Edge*> mEdges;
	};
}