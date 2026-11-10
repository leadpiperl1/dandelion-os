module DandelionRoute

using Sockets

export AbstractNodeState, BlueState, GreenState, RedState, route_packet!, start_ipc_server!

abstract type AbstractNodeState end
struct BlueState  <: AbstractNodeState end # -1: Intake / Buffering
struct GreenState <: AbstractNodeState end #  0: Inertial Ground
struct RedState   <: AbstractNodeState end # +1: Soliton Waveguide

struct Node
    id::UInt32
    state::AbstractNodeState
end

route_packet!(node::Node, ::BlueState, payload::Vector{UInt8}) = :buffered
route_packet!(node::Node, ::GreenState, payload::Vector{UInt8}) = :grounded
route_packet!(node::Node, ::RedState, payload::Vector{UInt8}) = :transmitted

end # module
