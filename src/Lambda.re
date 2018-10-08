/**
 * Lambda Handler example
 * https://github.com/prisma/graphql-yoga/blob/master/examples/lambda/handler.js
 */


module Prisma = {
  type t;

  [@bs.deriving abstract]
  type opts = {
    endpoint: string,
    [@bs.optional] debug: bool
  };

  [@bs.new]
  [@bs.module "prisma"]
  external newPrimsa : opts => t = "Prisma";

  /**
   *  the endpoint of the Prisma API (value set in `.env`)
   */
  [@bs.val]
  [@bs.scope ("process", "env")]
  external prismaEndpoint : string = "PRISMA_ENDPOINT";

  let defaultOpts : opts = opts(
    ~endpoint=prismaEndpoint,
    ~debug=true,
    ()
  );
};

module GraphQLServerLambda = {
  type server;
  type resolvers;
  type handler;

  [@bs.module "./resolvers"]
  external resolvers : resolvers = "";

  [@bs.deriving abstract]
  type extReq = {
    db: Prisma.t
  };

  [@bs.deriving abstract]
  type opts = {
    typeDefs: string,
    resolvers: resolvers,
    context: extReq => extReq
  };

  let defaultOpts = opts(
    ~typeDefs="./src/schema.graphql",
    ~resolvers=resolvers,
    ~context=(req => {
      ...req,
      db: Prisma.(newPrimsa(defaultOpts))
    })
  );

  [@bs.new]
  [@bs.module "graphql-yoga"]
  external newServer : opts => server = "GraphQLServerLambda";

  [@bs.get]
  external graphQLHandler : server => handler = "graphqlHandler";
}

/* 
type handler;

[@bs.deriving abstract]
type lambda = pri {
  graphqlHandler: handler,
  playgroundHandler: handler
};

[@bs.module "./setup"]
external setupLambda : unit => lambda = "setupLambda";

let lambda = setupLambda();

let server = lambda -> graphqlHandlerGet;
let playground = lambda -> playgroundHandlerGet; */