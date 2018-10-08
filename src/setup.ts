import { GraphQLServer, GraphQLServerLambda } from 'graphql-yoga'
import { Prisma } from './generated/prisma'
import resolvers from './resolvers'

const ServerOpts = {
  typeDefs: './src/schema.graphql',
  resolvers,
  context: req => ({
    ...req,
    db: new Prisma({
      endpoint: process.env.PRISMA_ENDPOINT, // the endpoint of the Prisma API (value set in `.env`)
      debug: true, // log all GraphQL queries & mutations sent to the Prisma API
      // secret: process.env.PRISMA_SECRET, // only needed if specified in `database/prisma.yml` (value set in `.env`)
    }),
  }),
};

export function setupDev() {
  return new GraphQLServer(ServerOpts)
}

export function setupLambda() {
  return new GraphQLServerLambda(ServerOpts)
}